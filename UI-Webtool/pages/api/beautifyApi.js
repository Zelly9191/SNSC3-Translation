const { DOMParser } = require('xmldom')

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const getErrors = (w) => {
        if (!w) {return;}
        try {
            const message = w.split('[xmldom warning]\t')[1].split('\n@#')[0]
            const line = w.split('[xmldom warning]\t')[1].split('\n@#')[1]

            arr.push({
                code: "Invalid",
                msg: message,
                line: (++line.split(',')[0].split('[line:')[1]) - 1,
                col: 1,
            })
        } catch(err) {
            console.error(err)
        }
    }

    let arr = [];

    var parser = new DOMParser({
        locator: {},
        errorHandler: { 
            warning: getErrors, 
            error: (e) => { console.log(e) }, 
            fatalError: (e) => { console.error(e) } 
        }
    });
    
    parser.parseFromString(req.body.xml, "application/xml");
    res.status(200).json({ beautified: arr });
};
