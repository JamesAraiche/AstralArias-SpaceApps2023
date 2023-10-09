<template>
  <div id="app">
    <h1>
      Cosmos PerSonified
    </h1>
  </div>
  <div class="image-container">
    <span v-for="n in 100" v-bind:key="n">
      <app-button class="hidden-button" :aud_file="n-1" buttonText="e" />
    </span>
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
.image-container {
  height: 800px;
  width: 800px;
  background-image: url("assets/Images/Image.png");
  background-size: contain;
}
.hidden-button {
  height: 10%;
  width: 10%;
  opacity: 0;
}

</style>
