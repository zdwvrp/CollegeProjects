import {IonAccordion, IonAccordionGroup, IonCard, IonCardSubtitle, IonCardTitle, IonCardContent, IonItem, IonLabel, IonList, IonIcon} from '@ionic/react';
import { checkmarkCircle, moon, sunny } from 'ionicons/icons';
import './css/GameCard.css';
import teamData from '../sampleData/teams';

const GameCard = (props: { data: any; }) => {

    const gameData = props.data;

    // homeWL: Away Team Win = 0, Home Team Win = 1
    const gamePrediction = () => {
        if( gameData.homeWL == 0 ) {
            return (
                <div  className="our-pick-element" id="prediction-away">
                    <IonIcon id="check-icon" ios={checkmarkCircle} md={checkmarkCircle} />
                    <p>Our Pick</p>
                </div>
            );
        } else if (gameData.homeWL == 1 ) {
            return (
                <div className="our-pick-element" id="prediction-home">
                    <IonIcon id="check-icon" ios={checkmarkCircle} md={checkmarkCircle} />
                    <p>Our Pick</p>
                </div>
            );
        }
    };

    const getTeamName = (teamId: string) => {
        const team = teamData.find(team => team.name === teamId)
        return team?.longName;
    }

    const getTeamColor = (teamId: string) => {
        const team = teamData.find(team => team.name === teamId)
        return team?.color;
    }
 

    const dayNight = () => {
        if( gameData.Day_Night == "N" ) {
            return (<IonIcon id="moon-icon" className="day-night-icon" ios={moon} md={moon} />);
        } else {
            return (<IonIcon id="sun-icon" className="day-night-icon" ios={sunny} md={sunny} />);
        }
    }

    return(
        <IonCard 
        className="game-card"
        style={{background: "linear-gradient(105deg, " + getTeamColor(gameData.away_teamID) + " 50%, " + getTeamColor(gameData.teamID) + " 50%)"}}>
            <div id="ion-card-title-wrapper">
                <IonCardTitle  
                className="ion-card-title"
                id="card-title-away-team">
                    {getTeamName(gameData.away_teamID)}
                </IonCardTitle>

                <IonCardTitle className="ion-card-title-at">@</IonCardTitle>

                <IonCardTitle 
                className="ion-card-title"
                id="card-title-home-team">
                    {getTeamName(gameData.teamID)}
                </IonCardTitle>
            </div>
            
            <IonCardSubtitle id="date-day-night-wrapper" className="ion-card-subtitle">
                {dayNight()}
                {gameData["date_d_Date_"]}
            </IonCardSubtitle>

            {gamePrediction()}

            <IonCardContent id="more-info-wrapper">
                <IonAccordionGroup id="accordion-group">
                    <IonAccordion id="accordion" value="More Info" >
                        <IonItem slot="header">
                            <IonLabel>Team Statistics</IonLabel>
                        </IonItem>
                        <IonList id="more-info-data-list" slot="content">
                            <IonItem>
                                <div className="card-row">
                                    <p className="away-more-info-data">{gameData.away_hit_average}</p>
                                    <p className="more-info-data-field">Avg Hits / Game</p>
                                    <p className="home-more-info-data">{gameData.home_hit_average}</p>
                                </div>
                            </IonItem>
                            <IonItem>
                                <div className="card-row">
                                    <p className="away-more-info-data">{gameData.away_run_average}</p>
                                    <p className="more-info-data-field">Avg Runs / Game</p>
                                    <p className="home-more-info-data">{gameData.homeRun_Average}</p>
                                </div>
                            </IonItem>
                            <IonItem>
                                <div className="card-row">
                                    <p className="away-more-info-data">{gameData.away_home_runs_average}</p>
                                    <p className="more-info-data-field">Avg HR / Game</p>
                                    <p className="home-more-info-data">{gameData.home_home_runs_average}</p>
                                </div>
                            </IonItem>
                            <IonItem>
                                <div className="card-row">
                                    <p className="away-more-info-data">{gameData.away_LOB_average}</p>
                                    <p className="more-info-data-field">Avg LOB / Game</p>
                                    <p className="home-more-info-data">{gameData.home_LOB_average}</p>
                                </div>
                            </IonItem>
                            <IonItem>
                                <div className="card-row">
                                    <p className="away-more-info-data">{gameData.away_walks_average}</p>
                                    <p className="more-info-data-field">Avg Walks / Game</p>
                                    <p className="home-more-info-data">{gameData.home_walks_average}</p>
                                </div>
                            </IonItem>
                        </IonList>
                    </IonAccordion>
                </IonAccordionGroup>
            </IonCardContent>
        </IonCard>
    );
};

export default GameCard;