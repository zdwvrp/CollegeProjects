import React, { useState } from "react";
import { auth, logInWithEmailAndPassword, logInWithGoogle } from "../utility/firebase";
import { IonButton, IonContent, IonInput, IonItem } from '@ionic/react';
import { useAuthState } from "react-firebase-hooks/auth";
import { Link, useHistory } from "react-router-dom";
import './css/Login.css'


function Login() {
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [user, loading, error] = useAuthState(auth);
    const history = useHistory();

    const loginFunction = (method: string, email?: string, password?: string) => {

      const path = '/pages/Home';

      if( method == 'google' ) {
        logInWithGoogle().then(() => {
          history.push(path);
        });

      } else if( method == 'email_password' && email && password) {
        logInWithEmailAndPassword(email, password).then(() => {
          history.push(path);
        });
      }
      
    };

    return (
      <IonContent className="login-wrapper">
          <IonItem id="please-note">
            Please Note: In our current build, the recommended login method is 'Login With Google'. 'Forgot Password' and 'Click here to Register' are Works in Progress.
          </IonItem>

          <IonInput
            type="email"
            className="login-textBox"
            pattern="email"
            value={email}
            onIonChange={(e) => setEmail(e.detail.value!)}
            placeholder="E-mail Address"
          />

          <IonInput
            type="password"
            className="login-textBox"
            pattern="password"
            value={password}
            onIonChange={(e) => setPassword(e.detail.value!)}
            placeholder="Password"
          />

          <IonButton
            color="secondary"
            className="login__btn"
            onClick={() => loginFunction("email_password", email, password)}
          >
            Login
          </IonButton>

          <IonButton 
            color="tertiary"
            className="login__btn login__google" 
            onClick={() => loginFunction("google")}
          >
            Login with Google
          </IonButton>

          <IonItem>
            <Link to="/reset">Forgot Password</Link>
          </IonItem>

          <IonItem>
            Don't have an account? <Link to="/register"> Click here to Register</Link>
          </IonItem>

      </IonContent>
    );
  }
  export default Login;