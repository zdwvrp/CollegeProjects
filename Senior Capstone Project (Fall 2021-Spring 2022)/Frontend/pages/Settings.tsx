import React, { useState } from "react";
import { IonAlert, IonButton, IonContent, IonItem, IonLabel, IonSelect, IonSelectOption } from '@ionic/react';
import TeamData from '../sampleData/teams';
import './css/Settings.css';
import { auth } from "../utility/firebase";
import firebase from "firebase/compat/app";
import 'firebase/compat/firestore';


const Settings: React.FC = ( name ) => {

    const [selectedTeam, setSelectedTeam] = useState("");
    const [submitButton, setSubmitButton] = useState(true);
    const [showAlert, setShowAlert] = useState(false);
    const [alertMessage, setAlertMessage] = useState("");

    const updateSelectedTeam = (value: string) => {
        setSubmitButton(false);
        setSelectedTeam(value);
    }

    const submit = async () => {
        const currentUser = auth.currentUser;
        if(currentUser) {
            try{
                firebase.firestore().collection("users")
                .where("uid", "==", currentUser.uid)
                .get()
                .then(function(querySnapshot) {
                    querySnapshot.forEach(function(document) {
                        document.ref.update({favTeam: selectedTeam});
                    });
                });
                setAlertMessage("Favorite Team successfully updated!");
                setShowAlert(true);
            } catch(e) {
                setAlertMessage("Unable to save changes: " + e);
                setShowAlert(true);
            }
        }
    }


    const teams = TeamData.map((data: {
        name: string
        longName: string;
        color: string;
    },
    key: any) => (
        <IonSelectOption className="team-item" value={data.name}>
            {data.longName}
        </IonSelectOption>
    ));

    return (
        <IonContent className="content-wrapper">
            <IonLabel id="select-team-label">Select Favorite Team</IonLabel>
            <IonItem id="select-team-item">
                <IonSelect id="select-team-select" value={selectedTeam} placeholder="Select Team" onIonChange={ev => updateSelectedTeam(ev.detail.value!)}>
                    {teams}
                </IonSelect>
            </IonItem>
            
            <IonButton id="submit-button" color="tertiary" disabled={submitButton} onClick={ () => submit()}>
                Save Changes
            </IonButton>

            <IonAlert 
                isOpen={showAlert} 
                subHeader={alertMessage}
                onDidDismiss={() => setShowAlert(false)}
                buttons = {[
                    {
                        text: 'Dismiss',
                        role: 'cancel',
                    }
                ]}
            />
        </IonContent>
    );

}

export default Settings;