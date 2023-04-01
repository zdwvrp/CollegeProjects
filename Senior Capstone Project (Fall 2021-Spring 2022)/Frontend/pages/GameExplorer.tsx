import React, {useEffect, useState} from 'react';
import {IonContent, IonSpinner} from '@ionic/react';
import { format, parseISO } from 'date-fns';
import DatePicker from '../components/DatePicker';
import GameCardList from '../components/GameCardList';
import './css/GameExplorer.css';

const GameExplorer: React.FC = () => {

    const newDate = new Date();

    // Set year to 2019 to account for the lack of current data
    newDate.setFullYear(2019);

    const defaultDate = newDate.toISOString();

    const [selectedDate, setSelectedDate] = useState(defaultDate);
    const [ data, setData ] = useState([]);
    const [ isLoading, setIsLoading ] = useState(false);

    useEffect(() => {
        async function fetchData() {
            const changedDate = new Date(selectedDate);
            var fetchDate = (changedDate.getMonth() + 1) + '/' + changedDate.getDate() + '/' + changedDate.getFullYear();
            setIsLoading(true);
            await fetch("http://localhost:3000/query?date_d_Date_=" + fetchDate, {
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
    }, [selectedDate]);

    const formatDate = (value: string) => {
        return format(parseISO(value), 'MMMM dd yyyy');
    };

    if( isLoading ) {
        return(
            <IonContent id="loading-overlay">
                <IonSpinner name="crescent" color="white" id="loading-spinner"/>
                <h3 id="loading-label">Loading Games...</h3>
            </IonContent>
        );
    } else {
        return (
            <IonContent id="content-wrapper">
                <div id="date-picker-wrapper">
                    <DatePicker selectedDate={selectedDate} 
                        changeDate={setSelectedDate} 
                        formatDate={formatDate}
                    />
                </div>
                <GameCardList data={data} />
            </IonContent>
        )
        
    }
};

export default GameExplorer;