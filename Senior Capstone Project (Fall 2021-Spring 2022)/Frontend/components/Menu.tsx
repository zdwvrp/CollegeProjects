import {
  IonContent,
  IonIcon,
  IonItem,
  IonLabel,
  IonList,
  IonListHeader,
  IonMenu,
  IonMenuToggle,
  IonNote,
} from '@ionic/react';

import { useLocation } from 'react-router-dom';
import {  
        homeOutline, homeSharp, 
        baseballOutline, baseballSharp, 
        bodyOutline, bodySharp,
        keyOutline, keySharp,
        settingsOutline, settingsSharp,
        searchOutline, searchSharp,
      } from 'ionicons/icons';

import React, { useEffect, useState } from "react";
import { auth, user } from '../utility/firebase';
import { onAuthStateChanged } from 'firebase/auth';
import './css/Menu.css';


interface AppPage {
  url: string;
  iosIcon: string;
  mdIcon: string;
  title: string;
  hideUntilLoggedIn: boolean;
  hideWhenLoggedIn: boolean;
}

const appPages: AppPage[] = [
  {
    title: 'Home',
    url: '/pages/Home',
    iosIcon: homeOutline,
    mdIcon: homeSharp,
    hideUntilLoggedIn: false,
    hideWhenLoggedIn: false,
  },
  {
    title: 'Games Today',
    url: '/pages/Games_Today',
    iosIcon: baseballOutline,
    mdIcon: baseballSharp,
    hideUntilLoggedIn: false,
    hideWhenLoggedIn: false,
  },
  {
    title: 'Game Explorer',
    url: '/pages/Game_Explorer',
    iosIcon: searchOutline,
    mdIcon: searchSharp,
    hideUntilLoggedIn: false,
    hideWhenLoggedIn: false,
  },
  {
    title: 'About',
    url: '/pages/About',
    iosIcon: bodyOutline,
    mdIcon: bodySharp,
    hideUntilLoggedIn: false,
    hideWhenLoggedIn: false,
  },
  {
    title: 'Login',
    url: '/pages/Login',
    iosIcon: keyOutline,
    mdIcon: keySharp,
    hideUntilLoggedIn: false,
    hideWhenLoggedIn: true,

  },
  {
    title: 'Settings',
    url: '/pages/Settings',
    iosIcon: settingsOutline,
    mdIcon: settingsSharp,
    hideUntilLoggedIn: true,
    hideWhenLoggedIn: false,
  },
];

const Menu: React.FC = () => {
  const location = useLocation();

  const [isLoggedIn, setIsLoggedIn] = useState(false);

  onAuthStateChanged(auth, (user) => {
      if(user) {
          setIsLoggedIn(true);
      } else {
          setIsLoggedIn(false);
      }
  });

  return (
    <IonMenu contentId="main" type="overlay">
      <IonContent>
        <IonList id="inbox-list">
          <IonListHeader>SPUD</IonListHeader>
          {appPages.map((appPage, index) => {
            if ( (!appPage.hideUntilLoggedIn && !isLoggedIn) || (!appPage.hideWhenLoggedIn && isLoggedIn) )
            return (
              <IonMenuToggle key={index} autoHide={false}>
                <IonItem className={location.pathname === appPage.url ? 'selected' : ''} routerLink={appPage.url} routerDirection="none" lines="none" detail={false}>
                  <IonIcon slot="start" ios={appPage.iosIcon} md={appPage.mdIcon} />
                  <IonLabel>{appPage.title}</IonLabel>
                </IonItem>
              </IonMenuToggle>
            );
          })}
        </IonList>
      </IonContent>
    </IonMenu>
  );
};

export default Menu;
