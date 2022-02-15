const char css[] PROGMEM =R"rawLiteral(
html {
font-family:Arial,Helvetica,sans-serif;
text-align:center;
}

h1 {
color:#fff;
font-size:1.4rem;
}

h2 {
color:#2e464e;
font-size:1.1rem;
font-weight:700;
margin:.2em;
}

.topnav {
background-color:#143642;
overflow:hidden;
}

.topnav img {
float:left;
margin:5px 10px;
max-width:55px;
}

body {
margin:0;
}

.content {
margin:0 auto;
max-width:600px;
padding:10px;
padding-top: 0;
padding-bottom: 0;
}

.card {
background-color:#F8F7F9;
box-shadow:2px 2px 12px 1px rgba(140,140,140,.5);
}

.button {
-khtml-user-select:none;
-moz-user-select:none;
-ms-user-select:none;
-webkit-tap-highlight-color:rgba(0,0,0,0);
-webkit-touch-callout:none;
-webkit-user-select:none;
background-color:#0f8b8d;
border:none;
border-radius:5px;
color:#fff;
font-size:1rem;
margin:5px;
outline:none;
padding:5px 10px;
text-align:center;
user-select:none;
}

.button:hover {
background-color:#14bdc0;
}

.button:active {
background-color:#0f8b8d;
box-shadow:2 2px #CDCDCD;
transform:translateY(2px);
}

.state {
color:#8c8c8c;
font-size:1rem;
font-weight:700;
}

input[type=number],select {
background-color:#9c9c9c;
border:0;
color:#fff;
font-size:.9rem;
font-weight:700;
text-align:center;
width:60%;
}

.configIP[type=text],select {
background-color:#9c9c9c;
border:0;
color:#fff;
font-size:.9rem;
font-weight:700;
padding:0;
text-align:center;
width:100%;
}

input[type=text]:disabled {
color:#6e6e6e;
}

table.gpiTable {
background-color:#9c9c9c;
border-collapse:collapse;
border-color:#adadad;
border-style:solid;
border-width:2px;
color:#fff;
width:100%;
}

input:hover {
background-color:#ccc;
}

table.RouterTable,table.interfaceTable {
background-color:#9c9c9c;
border-collapse:collapse;
border-color:#adadad;
border-style:solid;
border-width:0;
color:#fff;
width:100%;
}

table.RouterTable th {
width:33%;
}

table.gpiTable td,table.gpiTable th {
border-color:#adadad;
border-style:solid;
border-width:2px;
padding:0;
}

table.gpiTable thead {
background-color:#0091ff;
}

table.RouterTable thead,table.interfaceTable thead {
background-color:#003761;
padding:0;
}

footer {
background-color:#143642;
color:#fff;
font-size:.7rem;
padding:3px;
padding-right:10px;
text-align:right;
}
)rawLiteral";
