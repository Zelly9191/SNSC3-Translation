import beautify from "xml-beautifier";

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const beautifiedXmlText = beautify(req.body.xml);
    res.status(200).json({ beautified: beautifiedXmlText });
};
