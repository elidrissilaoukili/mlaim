<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Analog Clock</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
        }
        canvas {
            background-color: #fff;
            border: 2px solid #000;
            border-radius: 50%;
        }
    </style>
</head>
<body>
    <canvas id="clockCanvas" width="400" height="400"></canvas>

    <script>
        function drawClock() {
            const canvas = document.getElementById("clockCanvas");
            const ctx = canvas.getContext("2d");
            const radius = canvas.width / 2;
            ctx.translate(radius, radius);

            function drawFace(ctx, radius) {
                ctx.beginPath();
                ctx.arc(0, 0, radius, 0, 2 * Math.PI);
                ctx.fillStyle = 'white';
                ctx.fill();
                ctx.strokeStyle = '#333';
                ctx.lineWidth = 4;
                ctx.stroke();

                // Draw numbers
                ctx.font = `${radius * 0.15}px arial`;
                ctx.textBaseline = "middle";
                ctx.textAlign = "center";
                for (let num = 1; num <= 12; num++) {
                    const ang = num * Math.PI / 6;
                    ctx.rotate(ang);
                    ctx.translate(0, -radius * 0.85);
                    ctx.rotate(-ang);
                    ctx.fillText(num.toString(), 0, 0);
                    ctx.rotate(ang);
                    ctx.translate(0, radius * 0.85);
                    ctx.rotate(-ang);
                }
            }

            function drawTime(ctx, radius) {
                const now = new Date();
                let hour = now.getHours();
                let minute = now.getMinutes();
                let second = now.getSeconds();

                // Hour hand
                hour = hour % 12;
                hour = (hour * Math.PI / 6) + (minute * Math.PI / (6 * 60)) + (second * Math.PI / (360 * 60));
                drawHand(ctx, hour, radius * 0.5, radius * 0.07);

                // Minute hand
                minute = (minute * Math.PI / 30) + (second * Math.PI / (30 * 60));
                drawHand(ctx, minute, radius * 0.8, radius * 0.07);

                // Second hand
                second = (second * Math.PI / 30);
                drawHand(ctx, second, radius * 0.9, radius * 0.02, 'red');
            }

            function drawHand(ctx, pos, length, width, color = '#333') {
                ctx.beginPath();
                ctx.lineWidth = width;
                ctx.lineCap = "round";
                ctx.strokeStyle = color;
                ctx.moveTo(0, 0);
                ctx.rotate(pos);
                ctx.lineTo(0, -length);
                ctx.stroke();
                ctx.rotate(-pos);
            }

            function updateClock() {
                ctx.clearRect(-radius, -radius, canvas.width, canvas.height);
                drawFace(ctx, radius);
                drawTime(ctx, radius);
            }

            setInterval(updateClock, 1000);
        }

        drawClock();
    </script>
</body>
</html>
