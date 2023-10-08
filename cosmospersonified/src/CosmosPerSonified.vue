<template>
  <div id="app">
    <h1>
      Cosmos PerSonified
    </h1>
    <app-button @click="handleDark" :dark="true" buttonText="Dark" />
    <app-button @click="handleLight" style="margin-top: 10px;" buttonText="Light" />
  </div>
</template>

<script>
import AppButton from "./components/MyButton.vue"
export default {
  name: 'App',
  components: {
    AppButton
  },
  methods: {
        handleDark(){
            console.log("DARK");
        },
        handleLight(){
            console.log("LIGHT");
        }
    }
}
</script>

<style>
#app {
  display: flex;
  flex-direction: column;  
}
</style>
