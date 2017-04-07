/**
 * Created by foofybuster on 3/21/2017 AD.
 */

const {app, BrowserWindow, Menu, MenuItem} = require('electron')
const url = require('url')
const path = require('path')
let win
app.on('ready', () =>{
    let win = new BrowserWindow({
        width:800,
        height:700,
        resizable: false,
        frame: false
    })
    win.loadURL(`file://${__dirname}/loading/loading.html`)
    const menu = Menu.buildFromTemplate(template)
    Menu.setApplicationMenu(menu)
})

app.on('window-all-closed', () => {
    app.quit()
})

const template = [
    {
        label: 'Edit',
        submenu: [
            {
                role: 'undo'
            },
            {
                role: 'redo'
            },
            {
                type: 'separator'
            },
            {
                role: 'cut'
            },
            {
                role: 'copy'
            },
            {
                role: 'paste'
            }
        ]
    }
]
