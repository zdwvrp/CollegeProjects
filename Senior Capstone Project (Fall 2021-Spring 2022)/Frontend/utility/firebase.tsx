import firebase from 'firebase/compat/app';
import { getAuth, createUserWithEmailAndPassword, signInWithEmailAndPassword, signInWithPopup, GoogleAuthProvider, sendPasswordResetEmail, signOut, onAuthStateChanged } from "firebase/auth";
import { getFirestore, query, getDocs, collection, where, addDoc } from 'firebase/firestore';
import 'firebase/compat/auth';


var firebaseui = require('firebaseui');

var firebaseConfig = {
    apiKey: "AIzaSyBCgIlBcEuoF--9TydytN90b6QbeTu_M24",
    authDomain: "spud-7a868.firebaseapp.com",
    projectId: "spud-7a868",
    storageBucket: "spud-7a868.appspot.com",
    messagingSenderId: "836006763351",
    appId: "1:836006763351:web:4dd7298eef3077266a3c12",
    measurementId: "G-SPH0V0R72H"
};

var uiConfig = {
    callbacks: {
        signInSuccessWithAuthResult: function(authResult: any, redirectUrl: any) {
        // User successfully signed in.
        // Return type determines whether we continue the redirect automatically
        // or whether we leave that to developer to handle.
        return true;
        },
        uiShown: function() {
        // The widget is rendered.
        // Hide the loader.
        document.getElementById('loader')!.style.display = 'none';
        }
    },
    // Will use popup for IDP Providers sign-in flow instead of the default, redirect.
    signInFlow: 'popup',
    signInSuccessUrl: '<url-to-redirect-to-on-success>',
    signInOptions: [
        {
            provider: firebase.auth.GoogleAuthProvider.PROVIDER_ID,
            signInMethod: firebase.auth.EmailAuthProvider.EMAIL_LINK_SIGN_IN_METHOD
        }
    ],
};

const app = firebase.initializeApp(firebaseConfig);
const auth = getAuth(app);
const db = getFirestore(app);
const googleProvider = new GoogleAuthProvider();
const user = auth.currentUser;

const favTeam = async () => {
    if( auth.currentUser ) {
        try {
            const userData: any = [];
            await firebase.firestore().collection("users")
            .where("uid", "==", auth.currentUser.uid)
            .get()
            .then( (querySnapshot) => {
                querySnapshot.forEach(user => {
                    userData.push(user.data());
                })
            });
            return userData[0].favTeam;
        } catch (e) {
            throw e;
        }
    } else {
        return "ARI";
    }
};

var ui = new firebaseui.auth.AuthUI(firebase.auth());

const logInWithEmailAndPassword = async (email: string, password: string) => {
    try {
        await signInWithEmailAndPassword(auth, email, password);
    } catch (err: any) {
        console.error(err);
        alert(err.message);
    }
};


const logInWithGoogle = async () => {
  try {
    const res = await signInWithPopup(auth, googleProvider);
    const user = res.user;
    const q = query(collection(db, "users"), where("uid", "==", user.uid));
    const docs = await getDocs(q);
    if (docs.docs.length === 0) {
      await addDoc(collection(db, "users"), {
        uid: user.uid,
        name: user.displayName,
        authProvider: "google",
        email: user.email,
        favTeam: "default",
      });
    }
  } catch (err: any) {
    console.error(err);
    alert(err.message);
  }
};

const registerWithEmailAndPassword = async (name: string, email:string, password:string) => {
    try {
        const res = await createUserWithEmailAndPassword(auth, email, password);
        const user = res.user;
            await addDoc(collection(db, "users"), {
            uid: user.uid,
            name,
            authProvider: "local",
            email,
            favTeam: "default",
        });
    } catch (err: any) {
        console.error(err);
        alert(err.message);
    }
};

const sendPasswordReset = async (email: string) => {
    try {
        await sendPasswordResetEmail(auth, email);
        alert("Password reset link sent!");
    } catch (err: any) {
        console.error(err);
        alert(err.message);
    }
};

const logout = () => {
    signOut(auth);
};

export {
    auth,
    db,
    favTeam,
    logInWithGoogle,
    logInWithEmailAndPassword,
    registerWithEmailAndPassword,
    sendPasswordReset,
    logout,
    user,
  };