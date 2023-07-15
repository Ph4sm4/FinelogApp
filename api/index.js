const admin = require('firebase-admin');
const express = require('express');
const http = require('node:http');
const cors = require('cors');

const server = express();
const port = 4000;

var serviceAccount = require('./firebase-admin-sdk.json');

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: 'https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app',
});

server.use(express.json());
server.use(express.urlencoded({ extended: true }));
server.use(cors());

server.post('/adminDeleteUser', cors(), (req, res) => {
  const { localId } = req.body;
  console.log(localId);

  admin
    .auth()
    .deleteUser(localId)
    .then(() => {
      res.status(200).send('User deleted successfully');
    });
});

http.createServer(server).listen(port, () => {
  console.log(`⚡️[server]: Server is running at http://localhost:${port}`);
});
