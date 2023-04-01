import GameCard from './GameCard';
import './css/GameCard.css';


const GameCardList = ( props: { data: any; } ) => {

    const gameData = props.data;

    if (gameData.length == 0) {
        return(
            <>
                <p id="no-games-to-display">There are no games to display...</p>
            </>
        );
    }

    return(
        <>
            {gameData.map((game: any) => {
                return (<GameCard data={game} />);
            })}
        </>
    );
};

export default GameCardList;