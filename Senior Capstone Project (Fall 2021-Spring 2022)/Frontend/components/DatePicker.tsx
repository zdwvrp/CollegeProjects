import React from 'react';
import {IonButton, IonContent, IonDatetime, IonModal} from '@ionic/react';
import './css/DatePicker.css';

const DatePicker: React.FC<{selectedDate: string, changeDate: any, formatDate: any}> = ({
    selectedDate, 
    changeDate,
    formatDate
}) => {

    // const [datePickerValue, setDatePickerValue] = useState(selDate);

    const updateDate = (value: string) => {
        changeDate(value);
        //setDatePickerValue(value);
    }

    const dateString = formatDate(selectedDate);

    return(
        <div>
            <div id="date-header-wrapper">
                <h2>Showing games for {dateString}</h2>
                <IonButton id="open-modal" color="tertiary">
                    Change Date
                </IonButton>
            </div>
            
            <IonModal trigger="open-modal" backdropDismiss={false}>
                <IonContent id="date-time-wrapper">
                    <IonDatetime
                    id="date-time-picker"
                    presentation="date" 
                    showDefaultButtons={true} 
                    showDefaultTitle={true}
                    size="cover" 
                    value={selectedDate}
                    onIonChange={ev => updateDate(ev.detail.value!)}>
                        
                    </IonDatetime>
                </IonContent>
                
            </IonModal>
        </div>
        
        

    );
};

export default DatePicker;