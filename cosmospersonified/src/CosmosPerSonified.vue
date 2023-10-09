<template>
  <div class="container">
    <h1>
      Cosmos PerSonified
    </h1>
  </div>

  <div class="container">
    <p>A sonification based on the hyperspatial imaging of a photo of Mars. See github.com/JamesAraiche/AstralArias-SpaceApps2023</p>
  </div>

  <div class="container">
    <p>Click the image to begin</p>
  </div>

  <div class="container">
    <div class="image-container">
      <span v-for="n in 100" v-bind:key="n">
        <app-button class="hidden-button" :aud_file="n-1" buttonText="e" />
      </span>
    </div>
  </div>
</template>

<script>
import AppButton from "./components/MyButton.vue"

export default {
  name: 'App',
  data(){
    return{
      hover: false,
    };
  },

  components: {
    AppButton
  },
}
</script>

<style scoped>
.myDiv{
  border: 5px outset red;
}
#app {
  display: flex;
  flex-direction: column;  
}
.container {
  display: flex;
  justify-content: center;
}
.image-container {
  height: 600px;
  width: 600px;
  background-image: url("assets/Images/Image.png");
  background-size: contain;
  margin-top: 1em;
  border: 1px solid #ccc;
}
.hidden-button {
  height: 10%;
  width: 10%;
  opacity: 0;
}
p {
  margin: 0;
}
h1 {
  margin: 0.5em;
}

</style>
