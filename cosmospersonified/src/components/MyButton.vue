
<template>
    <button v-on="$attrs" @click="handleClick" :class="[dark ? 'dark' : 'light', 'baseButton']">{{buttonText}}</button>
</template>

<script>
export default {
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
};
</script>

<style>
.baseButton{
    padding: 10px;
    border: none;
    border-radius: 4px;
}
.light{
    background: white;
    color: black;
    border: 1px solid gray;
    cursor: pointer;
}

.dark{
    background: black;
    color: white;
    cursor:pointer;
}
</style>