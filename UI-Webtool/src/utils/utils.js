export const findLastIndexOfTarget = (array, target, limit) => {
    const joinedArray = array.slice(0, limit).join(' ');
    const lastIndexOfTarget = joinedArray.lastIndexOf(target);
    return lastIndexOfTarget;
}
