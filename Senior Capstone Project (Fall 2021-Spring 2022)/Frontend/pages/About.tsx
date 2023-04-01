import { IonContent, IonCard, IonCardHeader, IonCardTitle, IonCardSubtitle } from '@ionic/react';
import "./css/About.css";
const About: React.FC = () => {
    return(
        <IonContent>
            <IonCard>
                <IonCardHeader>
                    <IonCardTitle>SPUD</IonCardTitle>
                    <IonCardSubtitle>
                    SPUD or Sports Prediction Utility Dashboard is an application for predicting the outcomes of different sports related statistics, 
                    primarily for the sake of sports gambling (in jurisdictions where it is legal).
                    </IonCardSubtitle>
                </IonCardHeader>
                
            </IonCard>
            <h1>Meet the Team!</h1>
            <IonCard>
                <img src="assets/AUSTIN_CORTEVILLE.jpg" height="-50%" width="-50%"/>
                <IonCardHeader>
                    <IonCardTitle>Austin Corteville</IonCardTitle>
                    <IonCardSubtitle>Site Reliabity & Frontend Engineer</IonCardSubtitle>
                </IonCardHeader>
            </IonCard>
            <IonCard>
                <img src="assets/keaton_m.jpg" />
                <IonCardHeader>
                    <IonCardTitle>Keaton Meyer</IonCardTitle>
                    <IonCardSubtitle>Data Engineer</IonCardSubtitle>
                </IonCardHeader>
            </IonCard>
            <IonCard>
                <img src="assets/b_rich.jpg" />
                <IonCardHeader>
                    <IonCardTitle>Bailey Richards</IonCardTitle>
                    <IonCardSubtitle>Data Engineer</IonCardSubtitle>
                </IonCardHeader>
            </IonCard>
            <IonCard>
                <img src="assets/jacob_s.jpg" />
                <IonCardHeader>
                    <IonCardTitle>Jacob Salka</IonCardTitle>
                    <IonCardSubtitle>Backend and Data Engineer</IonCardSubtitle>
                </IonCardHeader>
            </IonCard>
            <IonCard>
                <img src="assets/zach_w.jpg" />
                <IonCardHeader>
                    <IonCardTitle>Zach Weinreich</IonCardTitle>
                    <IonCardSubtitle>Frontend/UX Engineer</IonCardSubtitle>
                </IonCardHeader>
            </IonCard>
            
        </IonContent>

    );
};

export default About;