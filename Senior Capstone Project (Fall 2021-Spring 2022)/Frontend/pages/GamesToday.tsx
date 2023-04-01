import React, { useEffect, useState } from 'react';
import { IonContent, IonLabel, IonSpinner } from '@ionic/react';
import GameCardList from '../components/GameCardList';
import "./css/GamesToday.css";

const styleWebformIframeEmbed: React.CSSProperties = {
    width: '1px',
    minWidth: '100%',
    minHeight: '100%'
};

const GamesToday: React.FC = ( name ) => {

    const [ data, setData ] = useState([]);
    const [ isLoading, setIsLoading ] = useState(false);

    useEffect(() => {
        const newDate = new Date();
        const defaultDate = (newDate.getMonth() + 1) + '/' + newDate.getDate() + '/2019';

        async function fetchData() {
            setIsLoading(true);
            await fetch("http://localhost:3000/query?date_d_Date_=" + defaultDate, {
                headers : {
                    'Content-Type': 'application/json',
                    'Accept' : 'application/json'
                }
                
            }).then(response => response.json())
            .then(data => setData(data))
            .then(() => setIsLoading(false));
        }
        fetchData();
        return;
    }, []);

    if( isLoading ) {
        return(
            <IonContent id="loading-overlay">
                <IonSpinner name="crescent" color="white" id="loading-spinner"/>
                <h3 id="loading-label">Loading Games...</h3>
            </IonContent>
        );
    } else {
        return (
            <IonContent>
                <GameCardList data={data} />
            </IonContent>
        )
        
    }
};

export default GamesToday;