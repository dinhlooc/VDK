const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <meta charset='utf-8'>
    <style>
      html {
        font-family: Helvetica;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      .button2 { background-color: #555555; }
      .btn {
        background-color: #4CAF50;
        border: none;
        color: white;
        padding: 16px 40px;
        text-decoration: none;
        font-size: 16px;
        margin: 2px;
        cursor: pointer;
      }
    </style>
    <title>Cảm biến khí gas và điều khiển động cơ DC</title>
  </head>
  <body>
    <h1>Giá trị khí gas đo được:</h1>
    <h2><p id="gasValue">Đang tải...</p></h2><br>

    <div id="motor_controller">
      <p>Điều khiển động cơ DC</p>
      <button class="btn btn-forward" onclick="goForward()">Tới</button>
      <button class="btn btn-backward" onclick="goBackward()">Lùi</button> <br>
    </div>

    <script>
      window.onload = function() {
        readGas();
      };

      function readGas() {
        var gasValue = document.getElementById('gasValue');
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            gasValue.innerHTML = this.responseText;
            setTimeout(readGas, 1000);
          }
        };
        xhttp.open('GET', '/readGas', true);
        xhttp.send();
      }

      function goForward() { 
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == 'go forward') {
              document.querySelector(".btn-forward").classList.add("button2");
              document.querySelector(".btn-backward").classList.remove("button2");
            }
          }
        };
        xhttp.open('GET', '/DCforward', true);
        xhttp.send();
      }

      function goBackward() { 
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == 'go backward') {
              document.querySelector(".btn-backward").classList.add("button2");
              document.querySelector(".btn-forward").classList.remove("button2");
            }
          }
        };
        xhttp.open('GET', '/DCbackward', true);
        xhttp.send();
      }
    </script>
  </body>
</html>
)=====";
