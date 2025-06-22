<template>
  <footer class="bottombar">
    <div class="footer-left">
      <span>&copy; {{ currentYear }} Stadtwerke Hünfeld</span>
    </div>

    <div class="footer-center">
      <el-link
        v-for="(link, index) in navLinks"
        :key="index"
        :href="link.href"
        target="_blank"
        class="footer-link"
      >
        {{ link.text }}
      </el-link>
    </div>

    <div class="footer-right">
      <el-link
        v-for="(icon, index) in socialIcons"
        :key="index"
        :href="icon.href"
        target="_blank"
        class="social-icon"
        :title="icon.name"
      >
        <Icon :icon="icon.name" width="20" height="20" />
      </el-link>
    </div>
  </footer>
</template>

<script setup>
import { Icon } from '@iconify/vue'

const currentYear = new Date().getFullYear()

const navLinks = [
  { text: 'Impressum', href: 'https://www.stadtwerke-huenfeld.de/impressum/' },
  { text: 'Datenschutz', href: 'https://www.stadtwerke-huenfeld.de/datenschutz/' },
]

const socialIcons = [
  { name: 'mdi:email', href: 'mailto:vertrieb@stadtwerke-huenfeld.de' },
]
</script>

<style scoped>
.bottombar {
  display: flex;
  flex-wrap: wrap;
  justify-content: space-between;
  align-items: center;
  background-color: #0168B3;
  color: white;
  padding: 1rem 2rem;
  font-size: 0.9rem;
  border-top: 1px solid #009EDF;
}

.footer-left,
.footer-center,
.footer-right {
  display: flex;
  align-items: center;
  gap: 1rem;
  flex-wrap: wrap;
}

.footer-center {
  flex: 1;
  justify-content: center;
}

.footer-link {
  color: #ffffff;
  font-weight: 500;
}

.footer-link:hover {
  text-decoration: underline;
}

.social-icon {
  color: #ffffff;
}

.social-icon:hover {
  color: #409EFF;
}
</style>
