<template>
  <div class="main-container">
    <TopBar />
    <div class="content">
      <router-view />
    </div>
    <BottomBar />
  </div>
</template>

<script>
import BottomBar from './components/BottomBar.vue';
import TopBar from './components/TopBar.vue'

export default {
  name: 'App',
  components: {
    TopBar,
    BottomBar
  },
}

</script>

<style>
body {
  margin: 0px;
  font-family: Arial, Helvetica, sans-serif;
  background: linear-gradient(to right, #f8f9fa, #e8ecf1);
}

.content {
  flex: 1; /* this pushes the BottomBar to the bottom */
}

.main-container {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

nav {
  padding: 1rem;
  background-color: #f0f0f0;
}

router-link {
  margin: 0 1rem;
  text-decoration: none;
  color: #2c3e50;
}

router-link-active {
  font-weight: bold;
  color: #1a2733;
}
</style>
