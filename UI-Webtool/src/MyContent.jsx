import React, { useState, useEffect, useRef } from 'react';
import ScriptEditor from './ScriptEditor';
import { Button, Input, Card, Col, Row, Upload, notification, TreeSelect, Switch, Tooltip } from 'antd';
import { UploadOutlined, DownloadOutlined, InfoCircleOutlined } from '@ant-design/icons';
import { saveAs } from 'file-saver';
import { xmlScript } from './sample';
import { errorScript } from './errors';
import { sampleTreeData } from './utils/tree';
import { useWindowSize } from "@uidotdev/usehooks";

const MyContent = () => {
    const monacoEditorRef = useRef(null);
    const editorRef = useRef(null);
    const [fullText, setFullText] = useState(xmlScript);
    const [lineNumbers, setLineNumbers] = useState([]);
    const [fileName, setFileName] = useState('sample');
    const [api, contextHolder] = notification.useNotification();
    const [treeData, setTreeData] = useState(sampleTreeData);
    const [loading, setIsLoading] = useState(false);
    const [decorations, setDecorations] = useState([]);
    const [isDarkTheme, setIsDarkTheme] = useState(true);
    const [enableErrorWindow, setEnableErrorWindow] = useState(false);

    useEffect(() => {
        setIsLoading(true)
        const getDirectoryList = async () => {
            try {
                const response = await fetch('/api/getDirectoryApi');
                const result = await response.json();
                setTreeData(result);
            } catch (error) {
                console.error('Error fetching data:', error);
            }            
        };

        getDirectoryList();
        setIsLoading(false)
    }, [])

    const onTreeSelect = async (value, label) => {
        const location = value.split('JE_SCRIPTS')[1]

        try {
            const response = await fetch(`/api/readDirectoryApi?${new URLSearchParams({fileName: location}).toString()}`);
            const result = await response.json();

            setFileName(label[0])
            setFullText(result.data)
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 5,
            });
        }
    };

    const validateXMLFile = async () => {
        try {
            const response = await fetch(`/api/validateFileApi`);
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
                message: 'Failed to highlight Line Number.',
                description:
                `Wrong Input...`,
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

    const applyCss = (effect) => {
        if (lineNumbers.length < 1) {
            api.error({
                message: 'Error',
                description:
                `Please input line numbers separated by comma`,
                duration: 5,
            });
            return;
        }

        const linesToEdit = lineNumbers.map((lineNumber) => ({
            range: {
                startLineNumber: lineNumber,
                endLineNumber: lineNumber,
            },
            options: {
                isWholeLine: true,
                marginClassName: effect,
                className: effect,
                hoverMessage: {value: 'Error here...'}
            }
        }))

        const decors = editorRef.current.deltaDecorations(decorations, linesToEdit);
        setDecorations(decors);
    }

    const renderButtons = () => {
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
                    setLineNumbers(toArray);
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

        return (
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
            </div>
        )
    }

    const size = useWindowSize();
    const isMobile = size?.width <= 870;

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
                        marginBottom: '55px'
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
                                    style={{width: '155px'}}
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
                                    icon={<DownloadOutlined />}
                                    type="primary"
                                    onClick={downloadXml}
                                >
                                    Download XML
                                </Button>
                            </div>
                        </div>
                    )}
                >
                    {renderButtons()}<br />
                    <div style={{display: 'flex', flexDirection: !isMobile ? 'row' : 'column'}}>
                        <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>Error Window</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked={false} 
                                onChange={(checked) => setEnableErrorWindow(checked)}
                            />
                        </div>
                        {!isMobile && <div>&nbsp;&nbsp;</div>}
                        <div style={{display: 'flex', flexDirection: 'row'}}>
                            {<p style={{marginTop: '5px'}}>Dark Mode</p>}
                            &nbsp;&nbsp;
                            <Switch
                                style={{marginTop: '5px', width: '30px'}}
                                defaultChecked 
                                onChange={(checked) => setIsDarkTheme(checked)}
                            />
                        </div>
                    </div><br />
                    <ScriptEditor
                        code={fullText}
                        setCode={setFullText}
                        editorOptions={{
                            stopRenderingLineAfter: 1000,
                            smoothScrolling: true
                        }}
                        monacoEditorRef={monacoEditorRef}
                        editorRef={editorRef}
                        isDarkTheme={isDarkTheme}
                        isMobile={isMobile}
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
                                <Tooltip placement="bottom" title={'You may also hover on the highlighted lines to view error description'}>
                                    <InfoCircleOutlined />
                                </Tooltip>
                                &nbsp;{'ERRORS'}
                            </div>
                        )}
                    >
                        {
                            errorScript.split('\n').map((line, index) => (
                                line 
                                ? 
                                    <div key={index}>
                                        {line}
                                    </div> 
                                : 
                                    <br/>
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