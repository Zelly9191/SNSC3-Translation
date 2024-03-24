const fs = require('fs');
const path = require('path');

export default async function handler(req, res) {
    if (req.method !== 'GET') {
        res.status(405).send({ message: 'Only GET requests allowed' })
        return;
    };

    const rootDir = path.join(process.cwd(), 'public', '/JE_SCRIPTS');
    const { fileName } = req.query;

    fs.readFile(rootDir + fileName, 'utf8', (err, data) => {
        if (!err) {
            res.status(200).json({data});
        }
    });
};
