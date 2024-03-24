const quillModules = {
    toolbar: [],
    clipboard: {
        matchVisual: false,
    },
};

const quillFormats = [
    'code-block',
    'code',
    'bold', 
    'italic', 
    'underline', 
    'strike',
    'align', 
    'list', 
    'indent',
    'size', 
    'header',
    'link', 
    'image', 
    'video',
    'color', 
    'background',
    'clean',
];

export const configQuill = {
    theme: 'snow',
    modules: quillModules,
    formats: quillFormats
}