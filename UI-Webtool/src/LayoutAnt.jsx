import { Layout, Tabs } from 'antd';
import React from 'react';
import MyContent from './MyContent'

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
          }}
        >
          Web Tools
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
                label: `JE Script Validator`,
                key: 1,
                children: <MyContent />,
              },
              {
                label: `Charts`,
                key: 2,
                children: <></>,
              }
            ]}
          />
        </Content>
      </Layout>
    </Layout>
  );
};

export default LayoutAnt;