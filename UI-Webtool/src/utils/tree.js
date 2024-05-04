export const processTree = (gitResult) => {
    const directories = gitResult.filter((data) => !data.path.includes('/'))
    
    const directoriesNoChildren = directories.map((data) => ({
        value: data.path,
        title: data.path,
        children: [],
        selectable: false,
    }));

    const getChildren = (parent) => {
        const children = gitResult.filter((data) => data.path.includes(parent))
        children.shift();

        return children.map((data) => ({
            value: data.path + '|' + data.sha,
            title: data.path.split('/')[1],
            children: [],
            selectable: true,
        }))
    };

    const directoriesWithChildren = directoriesNoChildren.map((data) => {
        return {
            value: data.value,
            title: data.value,
            children: getChildren(data.value),
            selectable: false,
        }
    });

    return {treeDirectory: directoriesWithChildren};
}
