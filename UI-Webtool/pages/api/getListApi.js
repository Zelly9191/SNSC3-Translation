import { Octokit } from "@octokit/core";

export default async function handler(req, res) {
    if (req.method !== 'POST') {
        res.status(405).send({ message: 'Only POST requests allowed' })
        return;
    }

    const GITHUB_TOKEN = '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';
    const jeFolderFileSha = '66b1ab1dfc6be745b65658761e6a8d3aeb9259e0'

    const octokit = new Octokit({
        auth: GITHUB_TOKEN
    })

    const url = `GET /repos/jack-fajardo/SNSC3/git/trees/${jeFolderFileSha}?recursive=1`;
    const params = {
        owner: 'jack-fajardo',
        repo: 'SNSC3',
        tree_sha: jeFolderFileSha,
        headers: {'X-GitHub-Api-Version': '2022-11-28'}
    }

    const response = await octokit.request(url, params)
    
    res.status(200).json({ response });
};
