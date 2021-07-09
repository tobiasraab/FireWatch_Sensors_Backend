if (process.env.NODE_ENV == 'production') {
    require('dotenv').config({ path: './.env' });
}


//environment variables
// Database
const dbUri = process.env.DB_URI.toString()
console.log(dbUri)


//modules
let mqtt = require('mqtt')
let mqttclient = mqtt.connect('mqtt://mqtt.hfg.design')

const MongoClient = require('mongodb').MongoClient
const uri = dbUri
let db
let data
const dbclient = new MongoClient(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})




// mqtt
mqttclient.on('connect', function () {
    mqttclient.subscribe('/sweavs/waldbrand/sensorstation_1', function (err) {
        if (!err) {
            console.log('Node connected to mqtt.hfg.design')
            mqttclient.publish('/sweavs/waldbrand/log', 'SensorBackend connected to mqtt broker')
        }
    })
})

mqttclient.on('message', function (topic, message) {
    saveData(message.toString())
})




// mongo
dbclient.connect(err => {
    console.log("Connected to Database")
    db = dbclient.db("FireWatch")
    data = db.collection("fireWatchData")
})



//functions
function saveData(message) {
    let doc = JSON.parse(message)
    doc.createdAt = new Date()
    if (data) {
        data.insertOne(doc, (err, res) => {
            const insertedCount = res.insertedCount
            console.log("inserted:     " + message)
        })
    }
}