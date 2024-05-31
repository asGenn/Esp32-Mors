/**
 * Import function triggers from their respective submodules:
 *
 * const {onCall} = require("firebase-functions/v2/https");
 * const {onDocumentWritten} = require("firebase-functions/v2/firestore");
 *
 * See a full list of supported triggers at https://firebase.google.com/docs/functions
 */

const {logger} = require("firebase-functions");
const {onRequest} = require("firebase-functions/v2/https");
const {onDocumentCreated} = require("firebase-functions/v2/firestore");

// The Firebase Admin SDK to access Firestore.
const {initializeApp} = require("firebase-admin/app");
const {getFirestore, Timestamp} = require("firebase-admin/firestore");
initializeApp();
const cors = require('cors');
const express = require('express');
const app = express();




app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  next();
});

app.get('/messages', async (req, res) => {
  const snapshot = await getFirestore().collection('messages').get();
  const messages = [];
  snapshot.forEach((doc) => {
    messages.push(doc.data());
  });
  res.send(messages);
});

app.get('/latest-message', async (req, res) => {
  try {
    const snapshot = await getFirestore().collection('messages')
      .orderBy('timestamp', 'desc')
      .limit(1)
      .get();

    if (snapshot.empty) {
      res.status(404).send('No messages found');as
      return;
    }

    const messages = [];
    snapshot.forEach(doc => {
      messages.push(doc.data());
    });

    res.send(messages[0]); // En son eklenen mesajı gönder
  } catch (error) {
    console.error('Error getting latest message:', error);
    res.status(500).send('Internal Server Error');
  }
});
app.get('/delete-message', async (req, res) => {
  try {
    await getFirestore().collection('messages').get().then((snapshot) => {
        snapshot.forEach((doc) => {
            doc.ref.delete();
        });
    });

    
    res.send('Messages deleted');
  } catch (error) {
    console.error('Error getting latest message:', error);
    res.status(500).send('Internal Server Error');
  }
});

exports.app = onRequest(app);