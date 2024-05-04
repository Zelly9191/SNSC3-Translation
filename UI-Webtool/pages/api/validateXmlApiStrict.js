const { XMLValidator } = require("fast-xml-parser");

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const result = XMLValidator.validate(`<xml>${req.body.xml}</xml>`, {
        allowBooleanAttributes: true,
        unpairedTags: []
    });

    res.status(200).send(result);
};
