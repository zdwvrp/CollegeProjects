import { IonButton, IonCard, IonContent, IonItem, IonLabel, IonList, IonRippleEffect } from "@ionic/react";
import React, { Component } from "react";

interface TabsProps{
    tabs:string[];
    currentTab:Function;
}

const Tabs: React.FC<TabsProps> = ({ tabs, currentTab }) => {
    let buttons = tabs.map((value, index) => 
                        <IonButton expand="block" fill="clear" key={index} className="ion-activatable ripple-parent">
                            {value}
                            <IonRippleEffect type="bounded"></IonRippleEffect>
                        </IonButton>
                );
    return (
        <IonContent>
            {buttons}
        </IonContent>
    );
};

export default Tabs;