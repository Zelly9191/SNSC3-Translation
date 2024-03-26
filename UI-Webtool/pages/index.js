import { useEffect, useState } from 'react'
import Layout from "../src/LayoutAnt"
import { Spin } from 'antd'

const IndexPage = () => {
    const [isLoading, setIsLoading] = useState();

    const CenteredSpinner = () => {
        const spinnerStyle = {
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          height: '100vh',
        };
      
        return (
          <div style={spinnerStyle}>
            <Spin size="large" />
          </div>
        );
    };

    useEffect(() => {
        setIsLoading(true);
        const delay = 1000; 

        const timeoutId = setTimeout(() => {
            setIsLoading(false);
        }, delay);

        return () => {
            clearTimeout(timeoutId);
        };
    }, []);

    
    if (isLoading) {
        return CenteredSpinner()
    }

    return <Layout />
};

export default IndexPage;