import React, { useState } from 'react';
import {IonHeader, IonToolbar, IonButtons, IonMenuButton, IonTitle, IonButton, IonPopover } from '@ionic/react';
import { auth, logout, user } from '../utility/firebase';
import { onAuthStateChanged } from 'firebase/auth';
import { useHistory } from 'react-router-dom';
import './css/Header.css';

interface HeaderProps {
    name: string,
}

const Header: React.FC<HeaderProps> = ({name}) => {

    const [isLoggedIn, setIsLoggedIn] = useState(false);
    const history = useHistory();


    onAuthStateChanged(auth, (user) => {
        if(user) {
            setIsLoggedIn(true);
        } else {
            setIsLoggedIn(false);
        }
    });

    const logoutFunction = () => {
        logout();
        const path = '/pages/Home'
        history.push(path);
    };

    const displayButtons = () => {
        if(isLoggedIn) {
            return (
                <IonButton id="logout-button" onClick={ () => logoutFunction()}>Logout</IonButton>
            );
        }
    }

    return ( 
            <IonHeader>
                <IonToolbar>
                    <IonButtons slot="start">
                        <IonMenuButton />
                    </IonButtons>
                    <IonTitle>{name}</IonTitle>
                    <IonButtons slot="end">
                        {displayButtons()}
                    </IonButtons>
                </IonToolbar>
            </IonHeader>
        );
}

export default Header;