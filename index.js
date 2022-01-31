var express = require('express')
var bodyParser = require('body-parser')
var app = express()
var cors = require('cors')

app.use(cors())
app.use(bodyParser.json())

app.set('port', (process.env.PORT || 4000))
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

;


app.get('/', function(req, res) {
    //console.log(req.query.data)


    let Data = req.query.data.split(',')
    let moss = Data[0]
    let suisei = Data[1]

    console.log(" DATA 1 ------- > " + moss + "    DATA 2 ------- > " + suisei)
    res.send("pong")
        /*var i = [4, 5, 6]
        console.log(i[1]);*/

    /* let Data = req.query.data
         let ss = req.query.ss
             //let str = Data[1]

         if (Data == "Suisei") {
             console.log("Suisei ------ yes")
             res.send("pong")
         } else res.send("Hello")*/

})

app.get('/suisei', function(req, res) {
    //console.log(req.query.data)


    let Data = req.query.data.split(',')
    let moss = Data[0]
    let suisei = Data[1]

    console.log(" DATA 1 ------- > " + moss + "    DATA 2 ------- > " + suisei)
    res.send("pong")
        /*var i = [4, 5, 6]
        console.log(i[1]);*/

    /* let Data = req.query.data
     let ss = req.query.ss
         //let str = Data[1]

     if (Data == "Suisei") {
         console.log("Suisei ------ yes")
         res.send("pong")
     } else res.send("Hello")*/

})



app.listen(app.get('port'), function() {
    console.log('run at port', app.get('port'))
})