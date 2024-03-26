const fs = require('fs');

export default async function handler(req, res) {
    if (req.method !== 'GET') {
        res.status(405).send({ message: 'Only GET requests allowed' })
        return;
    };

    const ENGRegexPrefix = "^[ \t]*[(0-9A-Za-z.,!?=:;'\\-\"*~　 )";
    const JPRegexPrefix = "^[ \t]*[(0-9ぁ-んァ-ヶ一-龥々ー?!？！、，:;\\-\"゠゠*~　 )";
    const whitelistTags = "(<three_dots>)*(<player_name>)*(<hearth>)*(<option_1>)*(<option_2>)*(<root>)*(<player_nickname>)*(<hearth>)*(<paw>)*(<partner>)*(<weapon_type>)*(<break>)*(<quote>)*(<music_note>)*(<lesser_than>)*(<greater_than>)*(<black_dot>)*(<left_arrow>)*";
    const regexSuffix = "]+<end_line>$";
    
    let report;
    process.setMaxListeners(0);

    function validateXML(xmlFile) {
        const { fileName } = req.query;

        const xmlPath = fileName;
        const xmlStream = fs.createReadStream(xmlPath);
        let currentLine;
        let lineCount = 0;
        let noErrors = true;
        let locationTag = false;

        if (!fs.existsSync(xmlPath)) {
            console.error("Corresponding .xml file does not exist or cannot be opened.\n");
            noErrors = false;
            return noErrors;
        }
        xmlStream.on('data', (chunk) => {
            const lines = chunk.toString().split('\n');
            for (let i = 0; i < lines.length; i++) {
                currentLine = lines[i];
                lineCount++;

                if (currentLine.includes("<location>")) {
                    locationTag = true;
                }
                if (currentLine.includes("<sjis>")) {
                    checkLines(currentLine, "<sjis>", noErrors, lineCount, locationTag);
                }
                if (currentLine.includes("<ascii>")) {
                    checkLines(currentLine, "<ascii>", noErrors, lineCount, locationTag);
                }
                locationTag = false;
            }
        });
        xmlStream.on('end', () => {
            xmlStream.close();
            if (!noErrors) {
                report += "The above errors are for the file: " + xmlFile + ".\n------------------------\n";
            }
            return noErrors;
        });
    }
    
    function checkLines(currentLine, tagType, lineCount, noErrors, locationTag) {
        let lines = 0;
console.log(currentLine)
        const prefix = (tagType === "<sjis>") ? JPRegexPrefix : ENGRegexPrefix;
        if (!currentLine.match(new RegExp(prefix + whitelistTags + regexSuffix))) {
            report += "ERROR: Incorrect match at line " + lineCount + ".\n";
            noErrors = false;
        }
        if (!currentLine.includes("　") && locationTag) {
            report += "Missing ideographic space in location tag.\n";
            noErrors = false;
        }
        lines++;
        if (lines > 3) {
            report += "Possible error detected! More than 3 lines of " + tagType + "text on line " + lineCount + ". Check for missing tags or overflowing text.\n";
            noErrors = false;
            
        }

        if (lines === 0) {
            report += "Empty " + tagType + " text at line " + lineCount + ".\n";
            noErrors = false;
        }             
    }
    
    validateXML();
    console.log(report);

    res.status(200).end()
};
