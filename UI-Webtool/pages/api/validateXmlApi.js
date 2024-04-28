const { XMLValidator } = require("fast-xml-parser");

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const ignoreTags = [
        "partner", 
        "end_line", 
        "three_dots",
        "player_name",
        "portrait_l",
        "portrait_r",
        "info",
        "player_nickname",
        "player"
    ]

    const result = XMLValidator.validate(`<xml>${req.body.xml}</xml>`, {
        allowBooleanAttributes: true,
        unpairedTags: ignoreTags
    });

    res.status(200).send(result);
};
