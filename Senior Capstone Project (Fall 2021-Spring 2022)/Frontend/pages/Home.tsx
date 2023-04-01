import React, { useEffect, useState } from 'react';
import { IonSpinner, IonContent } from '@ionic/react';
import GameCard from '../components/GameCard';
import GameCardList from '../components/GameCardList';
import { favTeam, auth } from '../utility/firebase';
import teamData from '../sampleData/teams'
import './css/Home.css'
import './css/Page.css'
const Home: React.FC = ( name ) => {

    const [ homeData, setHomeData ] = useState([]);
    const [ awayData, setAwayData ] = useState([]);
    const [ gotdData, setGotdData ] = useState([]);
    const [ userTeam, setUserTeam ] = useState("ARI");
    const [ isLoading, setIsLoading ] = useState(false);
    const [ isfinishedLoading, setIsFinishedLoading ] = useState(false);

    useEffect(() => {

        async function fetchData() {
            var queryTeam: any;
            const newDate = new Date();

            // Set year to 2019 to account for the lack of current data
            newDate.setFullYear(2019);

            const defaultDate = (newDate.getMonth() + 1) + '/' + newDate.getDate() + '/' + newDate.getFullYear();
            
            await favTeam().then(value => queryTeam = value)
            .then(() => setUserTeam(queryTeam));
            setIsLoading(true);

            await fetch("http://localhost:3000/query?date_d_Date_=" + defaultDate, {
                headers : {
                    'Content-Type': 'application/json',
                    'Accept' : 'application/json'
                }
            }).then(response => response.json())
            .then(data => setGotdData(data));

            await fetch("http://localhost:3000/query?date_d_Date_=" + defaultDate + "&away_teamID=" + queryTeam, {
                headers : {
                    'Content-Type': 'application/json',
                    'Accept' : 'application/json'
                }
            }).then(response => response.json())
            .then(data => setHomeData(data));

            await fetch("http://localhost:3000/query?date_d_Date_=" + defaultDate + "&teamID=" + queryTeam, {
                headers : {
                    'Content-Type': 'application/json',
                    'Accept' : 'application/json'
                }
            }).then(response => response.json())
            .then(data => setAwayData(data))
            .then(() => setIsLoading(false))
            .then(() => setIsFinishedLoading(true));
        }
        fetchData();
        return;
    }, []);

    const getTeamName = (teamId: string) => {
        const team = teamData.find(team => team.name === teamId)
        return team?.longName;
    }

    const showLoginMessage = () => {
        if( !auth.currentUser ) {
            return( <p id="login-message">Login and select your favorite team for their games to display here!</p>)
        }
    }

    if( !isfinishedLoading ) {
        return(
            <IonContent id="loading-overlay">
                <IonSpinner name="crescent" color="white" id="loading-spinner"/>
                <h3 id="loading-label">Loading Games...</h3>
            </IonContent>
        );
    } else {

        const randNum = Math.floor(Math.random() * gotdData.length);
        const upcomingGames = () => {
            if( homeData == [] && awayData != [] ) {
                return (<GameCardList data={awayData} />);
            } else if( homeData != [] && awayData == [] ) {
                return (<GameCardList data={homeData} />);
            } else {
                return (<GameCardList data={homeData.concat(awayData)} />);
            }
        };
        return (
            <IonContent>
                <>
                    <h1 id="game-of-the-day-header">Game of The Day</h1>
                    <GameCard data={gotdData[randNum]} />
                </>
                <>
                    <h1 id="fav-team-cards-header">{getTeamName(userTeam)} Game(s) Today</h1>
                    {showLoginMessage()}
                    {upcomingGames()}
                </>
            </IonContent>
        );
    }
}; 

export default Home;