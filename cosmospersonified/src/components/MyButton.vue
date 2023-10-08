
<template>
    <button v-on="$attrs" @mouseover="playAudio()" @mouseleave="stopAudio()" :class="['audiotest', 'baseButton']">{{buttonText}}</button>
</template>

<script>
const sound = require("@/assets/Audio/audio.wav")
var myTrack

export default {
    data: () => ({
        sound,
        myTrack
    }),

    props:{
        buttonText:{
            type: String,
            default: () => "Label",
        },
        dark:{
            type: Boolean,
            default: () => false,
        },
    },
    methods: {
        getAudio(){
        return this.sound;
        },
        playAudio(){
        myTrack = new Audio(this.sound)
        myTrack.addEventListener("canplaythrough", () => {
            myTrack.play();
        })
        },
        stopAudio(){
            myTrack.pause()
        },
    },
};
</script>

<style>
.baseButton{
    padding: 10px;
    border: none;
    border-radius: 4px;
}
.audiotest{
    background: white;
    color: black;
    border: 1px solid gray;
    cursor: pointer;
}

</style>