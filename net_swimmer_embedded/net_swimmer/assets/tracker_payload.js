(function() {
    function getCanvasFingerprint() {
        try {
            var canvas = document.createElement("canvas");
            var ctx = canvas.getContext("2d");
            canvas.width = 200; canvas.height = 50;
            ctx.font = "18pt Arial";
            ctx.fillStyle = "#FF0000";
            ctx.fillText("Fingerprint" + Math.random().toString(36).substring(2, 5), 10, 25);
            ctx.strokeStyle = "#00FF00";
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.arc(50, 25, 20, 0, Math.PI * 2, true);
            ctx.stroke();
            return canvas.toDataURL();
        } catch (e) {
            return "";
        }
    }

    function getWebGLFingerprint() {
        try {
            var canvas = document.createElement("canvas");
            var gl = canvas.getContext("webgl");
            if (!gl) return "";
            var debugInfo = gl.getExtension("WEBGL_debug_renderer_info");
            return [
                debugInfo ? gl.getParameter(debugInfo.UNMASKED_RENDERER_WEBGL) : "",
                gl.getParameter(gl.VERSION),
                gl.getParameter(gl.SHADING_LANGUAGE_VERSION)
            ].join(" | ");
        } catch (e) {
            return "";
        }
    }

    var result = {
        userAgent: navigator.userAgent,
        platform: navigator.platform,
        screen: screen.width + "x" + screen.height,
        languages: navigator.languages.join(","),
        canvas: getCanvasFingerprint(),
        webgl: getWebGLFingerprint()
    };

    print(JSON.stringify(result));
})();