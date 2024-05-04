{/* <Col className="gutter-row" span={12}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)',
                    }}
                    title={'React Syntax Highlighter'}
                >
                    <Input
                        placeholder="Input line numbers to highlight (comma separated e.g. 1, 2, 3, 4, 5)" 
                        onChange={(e) => {
                            const val = e.target.value;
                            try {
                                const toArray = val.split(',').map(Number);
                                setLineNumbers(toArray);
                            } catch (error) {
                                console.error('Wrong Input...')
                            }
                        }}
                    />
                    <br /><br />

                    <div style={{
                        height: '70vh',
                        overflow: 'scroll'
                    }}>
                        <SyntaxHighlighter
                            wrapLines={true}
                            showLineNumbers
                            lineProps={lineNumber => {
                                let style = { display: 'block' };
                                if (lineNumbers.includes(lineNumber)) {
                                    style.backgroundColor = '#FFC0CB';
                                }
                                return { style };
                            }}
                        >
                            {highlightedScript}
                        </SyntaxHighlighter>
                    </div>
                </Card>
            </Col> */}



            {/* <Col offset={1} span={8}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)'
                    }}
                    bodyStyle={{
                        height: '60vh',
                    }}
                    title={'Upload JE Script:'}
                >
                    
                </Card>
            </Col> */}

            {/* <Row>
            <Col offset={1} span={22}>
                <Card
                    style={{
                        boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)'
                    }}
                    bodyStyle={{
                        height: '13vh',
                    }}
                    title={'TODO:'}
                    size='small'
                >
                    <p>- it should be able to upload a JE script and put to editor</p>
                    <p>- it should be able to export corrected JE scripts</p>
                    <p>- it should be able to use a backend function to get the line numbers with error</p>
                    <p>- it should be able to use a show the error detail of a specific line number here on this container</p>
                </Card>
            </Col>


editorRef.current.createDecorationsCollection(
                [
                    {
                        range: {
                            startLineNumber: lineNumber,
                            endLineNumber: lineNumber,
                        },
                        options: {
                            isWholeLine: true,
                            marginClassName: effect,
                            className: effect,
                            hoverMessage: {value: 'Error here...'}
                        },
                    }
                ]
            );

        </Row> */}