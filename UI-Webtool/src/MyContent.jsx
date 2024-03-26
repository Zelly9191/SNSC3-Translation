import React, { useState, useEffect } from 'react';
import { Button, Input, Card, Col, Row, Upload, notification, TreeSelect } from 'antd';
import { UploadOutlined } from '@ant-design/icons';
import { useQuill } from 'react-quilljs';
import { configQuill } from './utils/config';
import { findLastIndexOfTarget } from './utils/utils';
import { saveAs } from 'file-saver';
import { xmlScript } from './sample';
import { errorScript } from './errors';
import { sampleTreeData } from './utils/tree';

const MyContent = () => {
    const [fullText, setFullText] = useState(xmlScript);
    const { quill, quillRef } = useQuill(configQuill);
    const [lineNumbers, setLineNumbers] = useState([]);
    const [fileName, setFileName] = useState('sample');
    const [api, contextHolder] = notification.useNotification();
    const [chosenFile, setChosenFile] = useState();
    const [treeData, setTreeData] = useState(sampleTreeData);
    const [loading, setIsLoading] = useState(false);

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

    useEffect(() => {
        if (quill) {
            quill.root.setAttribute("spellcheck", "false")
            quill.setContents({
                ops: [{ 
                    insert: fullText
                }]
            })

            quill.on('text-change', () => {
                setFullText(quill.getText());
            });
        }
    }, [quill]);


    const onTreeSelect = async (value, label) => {
        const location = value.split('JE_SCRIPTS')[1]
        setChosenFile(value);
        try {
            const response = await fetch(`/api/readDirectoryApi?${new URLSearchParams({fileName: location}).toString()}`);
            const result = await response.json();
            quill.setContents({
                ops: [{ 
                    insert: result.data
                }]
            })
            setFileName(label[0])
            setFullText(result.data)
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 10,
            });
        }
    };

    const validateXMLFile = async () => {
        try {
            const response = await fetch(`/api/validateFileApi?${new URLSearchParams({fileName: chosenFile}).toString()}`);
            const result = await response.json();
            console.log(result)
            
        } catch (error) {
            api.error({
                message: 'Something went wrong...',
                description: error,
                duration: 10,
            });
        }  
    };

    const highlighter = () => {
        try {
            quill.setContents({
                ops: [{ 
                    insert: fullText
                }]
            })
            const fullTextInArray = fullText.split('\n');
            if (lineNumbers.length > 0) {
                lineNumbers.forEach((line) => {
                    const realLine = line - 1;
                    quill.formatText(
                        findLastIndexOfTarget(fullTextInArray, fullTextInArray[realLine], line),
                        fullTextInArray[realLine].length,
                        {
                            'bold': true,
                            'background': '#FB5FFC',
                            'color': 'white',
                        }
                    );
                })
            }
        } catch (error) {
            api.error({
                message: 'Error',
                description:
                `Failed to highlight Line Number. ${error}`,
                duration: 10,
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
                    duration: 10,
                });
              return isXML;
            }

            setFileName(file.name)
            const reader = new FileReader();
            reader.readAsText(file);
            reader.onload = e => {
                const fileContent = e.target.result;
                quill.setContents({
                    ops: [{ 
                        insert: fileContent
                    }]
                })
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
            duration: 10,
        });
    }
    
    const renderButtons = () => (
        <div style={{ display: 'flex' }}>
            <Input
                style={{minWidth: '100px', maxWidth: '500px'}}
                placeholder="Line numbers to highlight (comma separated e.g. 1, 2, 3, 4, 5)" 
                onChange={(e) => {
                    const val = e.target.value;
                    try {
                        const toArray = val.split(',').map(Number);
                        setLineNumbers(toArray);
                    } catch (error) {
                        api.error({
                            message: 'Error',
                            description:
                            `Wrong input. ${error}`,
                            duration: 10,
                        });
                    }
                }}
            />
            &nbsp;&nbsp;
            <Button 
                type="primary" 
                onClick={highlighter}
            >
                Highlight
            </Button>
            &nbsp;&nbsp;
            <Button 
                type="primary" 
                onClick={validateXMLFile}
            >
                Validate
            </Button>
        </div>
    )

    return (
        !loading && <>
        {contextHolder}
        <Row gutter={16}>
            <Col className="gutter-row" span={18}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)'
                    }}
                    bodyStyle={{
                        height: '72vh',
                        marginBottom: '55px'
                    }}
                    title={'EDITOR'}
                    extra={renderButtons()}
                >
                    <div style={{display: 'flex', justifyContent: 'space-between'}}>
                        <div>
                            <TreeSelect
                                showSearch
                                style={{width: '170px'}}
                                dropdownStyle={{
                                    maxHeight: 900,
                                    overflow: 'auto',
                                    minWidth: 300,
                                }}
                                treeData={treeData?.treeDirectory}
                                placeholder="Search an XML File"
                                treeDefaultExpandAll={false}
                                onChange={onTreeSelect}
                            />
                            &nbsp;&nbsp; -- OR --&nbsp;&nbsp;
                            <Upload 
                                maxCount={1} 
                                listType='picture'
                                showUploadList={false}
                                {...uploadAndConvert}
                            >
                                <Button icon={<UploadOutlined />}>Browse XML File</Button>
                            </Upload>
                        </div>
                        &nbsp;&nbsp;
                        <Button 
                            type="primary"
                            onClick={downloadXml}
                        >
                            Export File
                        </Button>
                    </div>
                    <div ref={quillRef} />
                </Card>
            </Col>
            <Col className="gutter-row" span={6}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)',
                    }}
                    bodyStyle={{
                        height: '72vh',
                        marginBottom: '55px',
                        overflowY: 'scroll'
                    }}
                    title={'ERRORS'}
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
        </Row><br/>
        </>
    );
}

export default MyContent;