/**
 * Created by foofybuster on 4/7/2017 AD.
 */
var $ = require('jQuery');
$("#password").keypress(function(event) {
    if (event.which == 13) {
        add_va();
    }
});