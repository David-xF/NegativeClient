// ==UserScript==
// @name         Rule34 Image Downloader
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  makes easier to download images from rule34.xxx
// @author       mainCharacter
// @match        https://rule34.xxx/*
// @icon         https://www.google.com/s2/favicons?domain=rule34.xxx
// @connect      https://rule34.xxx/*
// @require      https://cdnjs.cloudflare.com/ajax/libs/jszip/3.2.0/jszip.min.js
// @require      https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/1.3.8/FileSaver.min.js
// @grant        GM_download
// @grant        GM_xmlhttpRequest
// ==/UserScript==

function transformToLink(filename, other) {
    return "https://wimg.rule34.xxx//images/" + other.split("/")[5] + "/" + filename.split(".")[0];
}

function downloadURI(uri, name) {
    var link = document.createElement("a");
    link.download = name;
    link.href = uri;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
    delete link;
}

function download_image(image_page_link) {
    let fileName_n = getFilenameFromLink(image_page_link);
    let fileName = fileName_n.split("_")[1];
    let link = transformToLink(fileName, image_page_link);
    var extensions = ["png", "jpeg", "jpg", "gif", "mp4"];

    extensions.forEach(function(extension, i) {
        var newLink = link + "." + extensions[i];
        let n = GM_download({
            url: newLink,
            name: fileName,
            saveAs: false
        });  

        console.log(n);
    });
}

function downloadAll(posts) {
    console.log(posts.length);
    posts.forEach(function(post) {
        let post_a = post.querySelector("a");
        let post_img = post_a.querySelector("img");
        download_image(post_img.src);
    });
}

function getFilenameFromLink(link){
    let result = link.slice(link.lastIndexOf("/")+1,link.lastIndexOf("?"));
    return result;
}
  
(function() {
    'use strict';
  
    let posts = document.querySelectorAll("span.thumb");
  
    let listElement = document.querySelector("ul.flat-list");
    let liElement = document.createElement('li');
    
    let showButtons = document.createElement('a');
    showButtons.innerHTML = "Show Custom Options";
    showButtons.onclick = function() {
        let downloadAllElement = document.createElement('a');
        downloadAllElement.innerHTML = "Download All Images";
        downloadAllElement.onclick = function() {
            downloadAll(posts);
        }
        liElement.appendChild(downloadAllElement);

        posts.forEach(function(post) {
            let button = document.createElement("button");
            button.innerHTML="<p  style='margin: 2px; font-size: 13px;' >" + "Download" + "</p>";
            button.style="color: white;	background-color: #1da1f2;	border: 0px white;	border-radius: 5px;margin: 2px;height: 32px;width: 100px";
            let link_n = post.querySelector("a");
            let link = post.querySelector("img").src;
            button.onclick = function() {
                download_image(link);
            }
            post.prepend(button);
        });
    }
    liElement.appendChild(showButtons);

    listElement.appendChild(liElement);
})();