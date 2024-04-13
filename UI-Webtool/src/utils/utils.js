export const findLastIndexOfTarget = (array, target, limit) => {
    const joinedArray = array.slice(0, limit).join(' ');
    const lastIndexOfTarget = joinedArray.lastIndexOf(target);
    return lastIndexOfTarget;
}

export const checkLines = (xmlStream, currentLine, tagType, lineCount, noErrors, report, locationTag) => {
    let lines = 0;
    let endTag = tagType;
    endTag = endTag.slice(0, 1) + "/" + endTag.slice(1);
    
    while (xmlStream.hasNextLine()) {
        currentLine = xmlStream.nextLine();
        lineCount++;
        if (currentLine.indexOf(endTag) === -1) {
            let prefix = (tagType === "<sjis>") ? JPRegexPrefix : ENGRegexPrefix;
            if (!currentLine.match(new RegExp(prefix + whitelistTags + regexSuffix))) {
                report += "ERROR: Incorrect match at line " + lineCount + ".\n";
                noErrors = false;
            }
            if (currentLine.indexOf("　") === -1 && locationTag) {
                report += "Missing ideographic space in location tag.\n";
                noErrors = false;
            }
            lines++;
        } else {
            break;
        }
        if (lines > 3) {
            report += "Possible error detected! More than 3 lines of " + tagType + "text on line " + lineCount + ". Check for missing tags or overflowing text.\n";
            noErrors = false;
            break;
        }
    }
    if (lines === 0) {
        report += "Empty " + tagType + " text at line " + lineCount + ".\n";
        noErrors = false;
    } else if (lines <= 3) {
        
    }
}
