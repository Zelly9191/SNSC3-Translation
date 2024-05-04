import { Octokit } from "@octokit/core";

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const { filePath } = req.body;
    const GITHUB_TOKEN = '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';

    const octokit = new Octokit({
        auth: GITHUB_TOKEN
    })

    const url = `GET /repos/jack-fajardo/SNSC3/contents/JE_SCRIPTS/${encodeURIComponent(filePath)}`;
    const params = {
        owner: 'jack-fajardo',
        repo: 'SNSC3',
        path: `JE_SCRIPTS/${filePath}`,
        headers: {'X-GitHub-Api-Version': '2022-11-28'}
    }

    const response = await octokit.request(url, params)
    
    res.status(200).json({ response });
};
