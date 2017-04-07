/**
 * Created by foofybuster on 3/26/2017 AD.
 */
var paths = [];
var file_name = [];
var opener = [];
function handleFileSelect(evt) {
    evt.stopPropagation();
    evt.preventDefault();

    var files = evt.dataTransfer.files; // FileList object.

    // files is a FileList of File objects. List some properties.

    var output = [];
    for (var i = 0, f; f = files[i]; i++) {
        output.push('<li><strong>', escape(f.name), '</strong> (', f.type || 'n/a', ') - ',
            f.size, ' bytes, last modified: ',
            f.lastModifiedDate ? f.lastModifiedDate.toLocaleDateString() : 'n/a',
            '</li>');
        paths.push(f.path);
        opener.push(f.path);
        console.log(f.path);
        file_name.push(f.name);
        console.log(f.name);
        opener = opener[opener.length-1].split(file_name[file_name.length-1]);
        console.log(opener)
    }
    document.getElementById('list').innerHTML = '<ul>' + output.join('') + '</ul>';
}
function handleDragOver(evt) {
    evt.stopPropagation();
    evt.preventDefault();
    evt.dataTransfer.dropEffect = 'copy'; // Explicitly show this is a copy.
}
var ourbox = document.getElementById('ourbox');
ourbox.addEventListener('dragover', handleDragOver);
ourbox.addEventListener('drop', handleFileSelect);