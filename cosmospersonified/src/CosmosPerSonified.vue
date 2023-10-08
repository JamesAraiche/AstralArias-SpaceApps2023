<template>
  <div id="app">
    <h1>
      Cosmos PerSonified
    </h1>
  </div>
  <div class="myDiv">
    <span v-for="n in 100" v-bind:key="n">
      <app-button :aud_file="n" buttonText="e" />
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
