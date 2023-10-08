<template>
  <div id="app">
    <h1>
      Cosmos PerSonified
    </h1>
  </div>
  <div class="myDiv">
    <span v-for="n in 1000">
      <app-button buttonText="" />
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
</style>
