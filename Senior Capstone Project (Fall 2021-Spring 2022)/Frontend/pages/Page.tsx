import { IonPage } from '@ionic/react';
import { useParams } from 'react-router';
import Header from '../components/Header';
import AboutComponent from './About';
import GameExplorerComponent from './GameExplorer';
import GamesTodayComponent from './GamesToday';
import HomeComponent from './Home';
import LoginComponent from './Login';
import SettingsComponent from './Settings';
import './css/Page.css';

const Page: React.FC = () => {



  var { name } = useParams<{ name: string; }>();
  if(name.includes('_')){
    let temp = name.split('_', 2);
    var name = temp[0]+" "+temp[1];
  }

  let pageComponent;

  switch(name){
    case "Home": 
      pageComponent = <HomeComponent/>;
      break;

    case "Games Today": 
      pageComponent = <GamesTodayComponent/>;
      break;

    case "About": 
      pageComponent = <AboutComponent/>;
      break;
      
    case "Game Explorer": 
      pageComponent = <GameExplorerComponent/>;
      break;
      
    case "Login": 
      pageComponent = <LoginComponent/>;
      break;

    case "Settings":
      pageComponent = <SettingsComponent/>;
      break;
      
    default: 
      pageComponent = <h1>There appears to be a routing error...</h1>;
  }

  return (
    <IonPage>
      <Header name={name}/>
      {pageComponent}
    </IonPage>
  );
};

export default Page;
