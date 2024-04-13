import Editor from '@monaco-editor/react';
import {
    useEffect,
} from 'react';

const ScriptEditor = (props) => {
    const { code, setCode, editorOptions, editorRef, monacoEditorRef, isDarkTheme, isMobile } = props;

    useEffect(() => {
        if (monacoEditorRef?.current) {
            const model = monacoEditorRef.current.getModels();

            if (model?.length > 0) {
                monacoEditorRef.current.setModelMarkers(model[0], 'owner', null)
            }
        }
    });

    return (
        <Editor
            height={isMobile ? '78%' : '93%'}
            language="xml"
            onChange={(value, _event) => {
                setCode(value)
            }}
            onMount={(editor, monaco) => {
                monacoEditorRef.current = monaco.editor
                editorRef.current = editor
            }}
            options={editorOptions}
            theme={isDarkTheme ? 'vs-dark' : 'vs'}
            value={code}
            width="auto"
        />
    );
}

export default ScriptEditor;
