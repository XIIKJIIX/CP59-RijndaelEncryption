/**
 * Created by foofybuster on 3/21/2017 AD.
 */

const electron = require('electron')  //use API
const {app, BrowserWindow} = electron
var path = require('path')

app.on('ready', () =>{
    let win = new BrowserWindow({width:800, height:700, resizable: false, frame: false, icon: path.join(__dirname, 'img/logo.png')})
    win.loadURL(`file://${__dirname}/start.html`)
})

