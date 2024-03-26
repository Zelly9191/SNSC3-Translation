const fs = require('fs');
const path = require('path');

export default async function handler(req, res) {
    if (req.method !== 'GET') {
        res.status(405).send({ message: 'Only GET requests allowed' })
        return;
    };

    const rootDir = path.join(process.cwd(), 'public', '/JE_SCRIPTS');
    const treeDirectory = [];

    function readDirectory(dirPath, parent) {
        const files = fs.readdirSync(dirPath);

        for (const file of files) {
            const filePath = path.join(dirPath, file);
            const isDirectory = fs.statSync(filePath).isDirectory();

            const entry = {
                value: filePath,
                title: file,
                children: [],
            };

            if (isDirectory) {
                readDirectory(filePath, entry);
            }

            parent.children.push(entry);
        }
    }

    readDirectory(rootDir, { children: treeDirectory });

    res.status(200).json({treeDirectory});
};
