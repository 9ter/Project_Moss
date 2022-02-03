var express = require('express')
var bodyParser = require('body-parser')
var app = express()
var cors = require('cors')
const LineAPI = require('line-api');
var mysql = require('mysql');
var date = require('date-utils');

const notify = new LineAPI.Notify({
    token: "****************"
})


app.use(cors())
app.use(bodyParser.json())

app.set('port', (process.env.PORT || 4000))
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: ""
});


app.get('/', function(req, res) {
    //console.log(req.query.data)


    let Data = req.query.data.split(',')
    let moss = Data[0]
    let suisei = Data[1]

    console.log(" DATA 1 ------- > " + Data[0] + "    DATA 2 ------- > " + Data[1])
    res.send("pong")
    
     
    
    con.query("SELECT * FROM 'ยังไม่ได้สร้าง' where ip = '" + Data[1] + "'", function(err, result, fields) {
    if (err) throw err;
    console.log("SELECT * FROM 'ยังไม่ได้สร้าง' where ip = '" + Data[1] + "'");
    console.log(result);
    
     notify.send({
            message: '' + name_lastname + ' สแกนแล้ว ' + Time_Stamp + "\n" + text_status
                //sticker: 'smile' // shorthan
                // sticker : { packageId: 1, id: 2 } // exact ids
                //image: 'test.jpg' // local file
                // image: { fullsize: 'http://example.com/1024x1024.jpg', thumbnail: 'http://example.com/240x240.jpg' } // remote url
        }).then(console.log)
    
    
       

})





app.listen(app.get('port'), function() {
    console.log('run at port', app.get('port'))
})
