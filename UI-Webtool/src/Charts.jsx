import { Chart as ChartJS, ArcElement, Tooltip, Legend } from "chart.js";
import { Card, Col, Row } from 'antd';
import { Pie } from "react-chartjs-2";

ChartJS.register(ArcElement, Tooltip, Legend);

const Charts = () => {
    const data = {
        labels: ['Untranslated', 'Translated'],
        datasets: [
          {
            label: 'Translation Progress',
            data: [12, 19],
            backgroundColor: [
              'rgba(255, 99, 132, 0.2)',
              'rgba(54, 162, 235, 0.2)',
            
            ],
            borderColor: [
              'rgba(255, 99, 132, 1)',
              'rgba(54, 162, 235, 1)',
              
            ],
            borderWidth: 1,
          },
        ],
      };
    return (
      <Row gutter={16}>
        <Col className="gutter-row" span={'24'}>
          <Card
            style={{
              boxShadow: '0px 4px 10px rgba(0, 0, 0, 0.2)'
            }}
            bodyStyle={{
              height: '72vh',
              marginBottom: '55px'
            }}
          >
            <Pie
              style={{height: '700px'}}
              data={data}
              options={{
                responsive: true,
                maintainAspectRatio: true
              }}
            />
          </Card>
        </Col>
      </Row>
    )
};

export default Charts;