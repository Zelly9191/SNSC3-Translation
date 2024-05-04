import React, { useState, useEffect, useRef } from 'react';
import ScriptEditor from './ScriptEditor';
import { Button, Input, Card, Col, Row, Upload, notification, TreeSelect, Switch, Tooltip } from 'antd';
import { UploadOutlined, DownloadOutlined, InfoCircleOutlined, CloudUploadOutlined } from '@ant-design/icons';
import { saveAs } from 'file-saver';
import { encode, decode } from 'js-base64';
import { processTree } from './utils/tree';
import { useWindowSize } from "@uidotdev/usehooks";
import fixIndents from "fix-indents";

const intro = `Select a file above to begin editing...\n
Files are saved in a test repo https://github.com/jack-fajardo/SNSC3`

const MyContent = () => {
    const monacoEditorRef = useRef(null);
    const editorRef = useRef(null);
    const [fullText, setFullText] = useState(intro);
    const [lineNumbers, setLineNumbers] = useState([]);
    const [fileName, setFileName] = useState('sample');
    const [fileSha, setFileSha] = useState('');
    const [filePath, setFilePath] = useState('');
    const [api, contextHolder] = notification.useNotification({ maxCount: 1 });
    const [treeData, setTreeData] = useState([]);
    const [loading, setIsLoading] = useState(false);
    const [decorations, setDecorations] = useState([]);
    const [isDarkTheme, setIsDarkTheme] = useState(true);
    const [enableErrorWindow, setEnableErrorWindow] = useState(true);
    const [enableDebug, setEnableDebug] = useState(false);
    const [enableStrict, setEnableStrict] = useState(true);
    const [hasThereBeenChanges, setHasThereBeenChanges] = useState(false);
    const NO_ERRORS = 'No errors found! :)';

    useEffect(() => {
        setIsLoading(true)
        getGithubContents();
        setIsLoading(false)
    }, [])

    const onTreeSelect = async (value, label) => {
        try {
            const url = '/api/getApi';
            const path = value.split('|')[0];

            const response = await fetch(url, {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({ filePath: path })
            })
            const result = await response.json();
            
            if (result?.response?.status === 200) {
                setFileSha(result.response.data.sha)
                setFullText(decode(result.response.data.content))
                setFilePath(path)
                setFileName(result.response.data.name)
                setHasThereBeenChanges(false)
                setLineNumbers([])
                applyCss("")

                api.success({
                    message: `File loaded`,
                    description: `File ${result.response.data.name} successfully fetched and loaded`,
                    duration: 5,
                });
                return;
            }
            api.error({
                message: 'Failed to fetch file',
                description: 'Please try again...',
                duration: 5,
            });
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 5,
            });
        }
    };

    const parseValidationReponse = (err) => {
        if (!err) {
            setLineNumbers([
                {
                    code: "Invalid",
                    line: 0, 
                    msg: NO_ERRORS,
                    col: 1
                }
            ]);
            applyCss("");
            return;
        };
        applyCss("highlight", [err])
        setLineNumbers([err]);
    }

    const validateXMLFile = async () => {
        const urlStrict = '/api/validateXmlApiStrict';
        const urlnonStrict = '/api/validateXmlApi';
        const url = enableStrict ? urlStrict : urlnonStrict;
        try {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({xml: fullText})
            })
            const result = await response.json();
            parseValidationReponse(result.err);
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 5,
            });
        }  
    };

    const highlighter = () => {
        try {
            if (editorRef?.current) {
                applyCss("highlight");
            }
        } catch (error) {
            api.error({
                message: 'Failed to highlight',
                description:
                `Wrong Input... ${error}`,
                duration: 5,
            });
        }
    }

    const uploadAndConvert = {
        beforeUpload(file) {
            const isXML = file.type === 'text/xml';
            if (!isXML) {
                api.error({
                    message: 'Error',
                    description: `File '${file.name}' is invalid. Please upload a valid JE Script.`,
                    duration: 5,
                });
              return isXML;
            }

            setFileName(file.name)
            setFileSha('')
            setFilePath('')
            setHasThereBeenChanges(false)
            const reader = new FileReader();
            reader.readAsText(file);
            reader.onload = e => {
                const fileContent = e.target.result;
                setFullText(fileContent);
            };
            return false;
        },
    };

    const downloadXml = () => {
        if (fullText.length > 1) {
            const blob = new Blob([fullText], { type: 'text/xml' });
            saveAs(blob, fileName);
            return;
        }
        api.warning({
            message: 'Editor is empty...',
            description: ``,
            duration: 5,
        });
    }

    const saveToGithub = async () => {
        const url = '/api/saveApi';
        try {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    content: encode(fullText),
                    filePath: filePath,
                    fileSha: fileSha
                })
            })
            const result = await response.json();

            if (result?.response?.status === 200) {
                setFileSha(result?.response?.data.content.sha)
                setHasThereBeenChanges(false)
                api.success({
                    message: 'File saved!',
                    description: `Changes successfully saved to github! ${result.response.url}`,
                    duration: 5,
                });
                return;
            }
            api.error({
                message: 'Failed to save file',
                description: 'Please try again...',
                duration: 5,
            });
        } catch (error) {
            api.error({
                message: 'Failed to save JE Script :(',
                description: error,
                duration: 5,
            });
        }  
    }

    const saveXml = () => {
        if (fullText.length > 1) {
            saveToGithub();
            return;
        } 
        api.warning({
            message: 'Editor is empty...',
            description: ``,
            duration: 5,
        });
    }

    const applyCss = (effect, data = []) => {
        const linesToHighlight = data.length > 0 ? data : lineNumbers;
        const linesToEdit = linesToHighlight.map(({line, msg}) => ({
            range: {
                startLineNumber: line,
                endLineNumber: line,
            },
            options: {
                isWholeLine: true,
                marginClassName: effect,
                className: effect,
                hoverMessage: {value: msg}
            }
        }))
        const decors = editorRef.current.deltaDecorations(decorations, linesToEdit);
        setDecorations(decors);
    }

    const fixIndent = async () => {
        const fixedCode = fixIndents(fullText, {"countSpaces": 2})
        setFullText(fixedCode);
        // editorRef.current.trigger("anyString", 'editor.action.formatDocument');
        // editorRef.current.getSupportedActions().forEach((value) => {
        //     console.log(value);
        // });
    };

    const beautifyXml = async () => {
        const url = '/api/beautifyApi';
        try {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({xml: fullText})
            })
            const result = await response.json();
            if (result.beautified.length > 0) {
                applyCss("highlight", result.beautified)
                setLineNumbers(result.beautified);
            }
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 5,
            });
        }  
    };

    const getGithubContents = async () => {
        try {
            const url = '/api/getListApi';
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({ url: url })
            })
            const result = await response.json();

            if (result?.response?.status === 200) {
                api.success({
                    message: 'File list updated!',
                    description: 'Successfully fetched file list from github!',
                    duration: 5,
                });
                const treeData = processTree(result.response.data.tree || [])
                setTreeData(treeData);
                return;
            }

            api.error({
                message: 'Failed to fetch files... Please refresh browser...',
                description: error,
                duration: 5,
            });
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: 'Please refresh your browser',
                duration: 5,
            });
        }  
    };

    const renderButtons = () => {
        const validateBtn = (
            <Button 
                type="primary" 
                onClick={validateXMLFile}
                style={{visibility: enableDebug ? 'visible' : 'hidden'}}
            >
                Validate Tags Once
            </Button>
        )

        const fixIndentBtn = (
            <Button 
                type="primary" 
                onClick={fixIndent}
                style={{visibility: enableDebug ? 'visible' : 'hidden'}}
            >
                Fix Indentations
            </Button>
        )

        const beautifyBtn = (
            <Button 
                type="primary" 
                onClick={beautifyXml}
                disabled={!enableStrict}
            >
                Validate All Tags
            </Button>
        )

        const getGithubContentsBtn = (
            <Button 
                type="primary" 
                onClick={() => getGithubContents()}
            >
                Refetch File List From Github
            </Button>
        )

        return (
            <div>
                {/* {validateBtn}
                &nbsp;&nbsp; */}
                {beautifyBtn}
                &nbsp;&nbsp;
                {getGithubContentsBtn}
                &nbsp;&nbsp;
                {fixIndentBtn}
            </div>
        )
    }

    const highlightBtn = (
        <Button 
            type="primary" 
            onClick={highlighter}
        >
            Highlight
        </Button>
    )

    const removeBtn = (
        <Button 
            type="primary" 
            onClick={() => {
                if (editorRef?.current) {
                    applyCss("");
                }
            }}
        >
            Remove
        </Button>
    )

    const onChangeHandler = (e) => {
        const val = e.target.value;
        try {
            const toArray = val.split(',').map(Number);
            if (val) {
                const numbersToObjects = toArray.map((num) => (
                    {
                        code: "Invalid",
                        msg: "sample error message",
                        line: num,
                        col: 1
                    }
                ))
                setLineNumbers(numbersToObjects);
            } else {
                setLineNumbers([]);
            }
        } catch (error) {
            api.error({
                message: 'Error',
                description:
                `Wrong input. ${error}`,
                duration: 5,
            });
        }
    }

    const size = useWindowSize();
    const isMobile = size?.width <= 870;

    const onEditorChange = (value, _event) => {
        setHasThereBeenChanges(true);
        applyCss("");
        setFullText(value);
    }

    return (
        !loading && <>
        {contextHolder}
        <Row gutter={16}>
            <Col className="gutter-row" span={isMobile || !enableErrorWindow ? '24' : '18'}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)'
                    }}
                    bodyStyle={{
                        height: '72vh',
                        marginBottom: !isMobile ? '55px' : '125px'
                    }}
                    title={(
                        <div style={{
                            display: 'flex', 
                            justifyContent: 'space-between', 
                            paddingTop: isMobile && '15px',
                            paddingBottom: isMobile && '15px'
                        }}>
                            <div style={{display: 'flex', flexDirection: isMobile ? 'column' : 'row'}}>
                                <TreeSelect
                                    showSearch
                                    style={{width: isMobile ? '155px' : '300px'}}
                                    dropdownStyle={{
                                        maxHeight: 900,
                                        overflow: 'auto',
                                        minWidth: 300,
                                    }}
                                    listHeight={300}
                                    treeData={treeData?.treeDirectory}
                                    placeholder="Search an XML File"
                                    treeDefaultExpandAll={false}
                                    onChange={onTreeSelect}
                                />
                                
                                {!isMobile ? (<div>&nbsp;&nbsp;</div>) : <br />}
                                {!isMobile && <p style={{marginTop: '5px'}}>-- OR --</p>}
                                {!isMobile && (<div>&nbsp;&nbsp;</div>)}
                                
                                <Upload 
                                    maxCount={1}
                                    showUploadList={false}
                                    {...uploadAndConvert}
                                >
                                    <Button icon={<UploadOutlined />}>Browse XML File</Button>
                                </Upload>
                            </div>
                            <div style={{display: 'flex', flexDirection: isMobile ? 'column' : 'row'}}>
                                <Button
                                    icon={<CloudUploadOutlined />}
                                    type="primary"
                                    onClick={saveXml}
                                    disabled={fileName === 'sample' || !fileSha || !filePath || !hasThereBeenChanges}
                                >
                                    Save JE Script
                                </Button>
                                {!isMobile ? (<div>&nbsp;&nbsp;</div>) : <div style={{marginTop: '25px'}}/>}
                                <Button
                                    icon={<DownloadOutlined />}
                                    type="primary"
                                    onClick={downloadXml}
                                >
                                    Export JE Script
                                </Button>
                            </div>
                        </div>
                    )}
                >
                    <div style={{display: 'flex', flexDirection: !isMobile ? 'row' : 'column'}}>
                        <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>
                            <Tooltip placement="bottom" title={'Shows error window that contains links to lines with errors'}>
                                <InfoCircleOutlined />
                            </Tooltip> Error Window</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked
                                onChange={(checked) => setEnableErrorWindow(checked)}
                            />
                        </div>
                        {!isMobile && <div>&nbsp;&nbsp;</div>}
                        <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>
                            <Tooltip placement="bottom" title={'Protect your eyes :)'}>
                                <InfoCircleOutlined />
                            </Tooltip> Dark Mode</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked 
                                onChange={(checked) => setIsDarkTheme(checked)}
                            />
                        </div>
                        {!isMobile && <div>&nbsp;&nbsp;</div>}
                        <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>
                            <Tooltip placement="bottom" title={'Shows highlighter inputs for ease of development. Also shows wonky future features. For devs only.'}>
                                <InfoCircleOutlined />
                            </Tooltip> Debugger</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked={false}
                                onChange={(checked) => setEnableDebug(checked)}
                            />
                        </div>
                        {!isMobile && <div>&nbsp;&nbsp;</div>}
                        {/* <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>
                            <Tooltip placement="bottom" title={`Strict validation rules. All open tags should have a corresponding closing tag. 
                                If disabled, 
                                only sjis and ascii tags will be validated`
                            }>
                                <InfoCircleOutlined />
                            </Tooltip> Strict Mode</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked
                                onChange={(checked) => setEnableStrict(checked)}
                            />
                        </div> */}
                    </div><br />
                    {renderButtons()}<br/>
                    <ScriptEditor
                        code={fullText}
                        editorOptions={{
                            stopRenderingLineAfter: 1000,
                            smoothScrolling: true,
                            tabSize: 2,
                        }}
                        monacoEditorRef={monacoEditorRef}
                        editorRef={editorRef}
                        isDarkTheme={isDarkTheme}
                        isMobile={isMobile}
                        onEditorChange={onEditorChange}
                    />
                </Card>
            </Col>
            {isMobile && <div>&nbsp;&nbsp;</div>}
            { enableErrorWindow &&
                <Col className="gutter-row" span={isMobile ? '24' : '6'}>
                    <Card
                        style={{
                            boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)',
                        }}
                        bodyStyle={{
                            height: '72vh',
                            marginBottom: '55px',
                            overflowY: 'scroll'
                        }}
                        title={(
                            <div>
                                <Tooltip placement="bottom" title={'Click the line number to automatically navigate the editor to that line. You may also hover on the highlighted lines to view error description'}>
                                    <InfoCircleOutlined />
                                </Tooltip>
                                &nbsp;{'ERRORS'}
                            </div>
                        )}
                    >
                        {enableDebug &&
                            <div style={{ display: 'flex' }}>
                                <Input
                                    style={{minWidth: '100px', maxWidth: 'auto'}}
                                    placeholder="Line numbers to highlight (comma separated e.g. 1, 2, 3, 4, 5)" 
                                    onChange={onChangeHandler}
                                />
                                &nbsp;&nbsp;
                                {highlightBtn}
                                &nbsp;&nbsp;
                                {removeBtn}
                                &nbsp;&nbsp;
                            </div>
                        }
                        {enableDebug && <br/>}
                        {
                            lineNumbers.map(({line, msg, col}, index) => (
                                <div key={index}>
                                    {msg}<br/>
                                    {
                                        msg !== NO_ERRORS &&
                                        <>
                                            {`Go to line number: `}
                                            <a style={{
                                                textDecoration: 'underline',
                                                color: 'blue'
                                            }} onClick={() => {
                                                editorRef?.current.revealLineInCenter(line);
                                                editorRef?.current.setPosition({ lineNumber: line, column: col });
                                            }}>
                                                {line}
                                            </a>
                                        </>
                                    }
                                    <br/><br/>
                                </div> 
                            ))
                        }
                    </Card>
                </Col>
            }
        </Row><br/>
        </>
    );
}

export default MyContent;