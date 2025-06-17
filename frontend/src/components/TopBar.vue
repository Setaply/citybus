<template>
  <nav class="topbar">
    <div class="nav-left">
      <router-link to="/" class="nav-link">
        <img src="@/assets/HünfeldLogo.svg" alt="Logo" class="logo-image" />
      </router-link>
    </div>
    <div class="nav-right">
      <h3>Hünfeld Citybus GPS</h3>
    </div>
  </nav>
</template>

<script setup>
</script>

<style scoped>
.topbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1rem 2rem;
  background-color: #0168B3;
  color: white;
  border-bottom: 1px solid #009EDF;
}

.nav-left,
.nav-right {
  display: flex;
  align-items: center;
}

.nav-link {
  color: white;
  text-decoration: none;
  font-size: 1.2rem;
  font-weight: bold;
  margin-right: 1rem;
}

.nav-link:hover {
  text-decoration: underline;
}

.logo-image {
  height: 40px; /* adjust as needed */
  cursor: pointer;
}

.button {
  background-color: #3498db;
}
</style>
