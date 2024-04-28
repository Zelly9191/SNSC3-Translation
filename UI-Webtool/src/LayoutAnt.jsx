import { Layout, Tabs } from 'antd';
import React from 'react';
import MyContent from './MyContent'
import Charts from './Charts';

const { Header, Content } = Layout;

const LayoutAnt = () => {
  return (
    <Layout>
      <Header
        style={{
          height: '80px',
          display: 'flex',
          alignItems: 'center',
        }}
      >
        <img
          src={'/logo.png'} 
          alt='logo' 
          width={130}
          height={70}
          style={{
            marginLeft: '-17px'
          }}
        />
        &nbsp;&nbsp;&nbsp;
        <h1
          style={{
            color: 'white',
            fontSize: '25px',
            letterSpacing: '2px',
          }}
        >
          <i>Web Tools v1</i>
        </h1>
      </Header>
      <Layout>
        <Content>
          <Tabs
            type='line'
            tabPosition={'top'}
            style={{
              marginLeft: '20px',
              marginRight: '20px',
              fontSize: '20px'
            }}
            items={[
              {
                label: <b>JE SCRIPT VALIDATOR</b>,
                key: 1,
                children: <MyContent />,
              },
              {
                label: <b>CHARTS</b>,
                key: 2,
                children: <div style={{paddingBottom: '50px'}}>
                  <Charts />
                </div>,
              }
            ]}
          />
        </Content>
      </Layout>
    </Layout>
  );
};

export default LayoutAnt;