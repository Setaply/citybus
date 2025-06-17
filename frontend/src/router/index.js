import Homepage from '@/pages/Homepage.vue'
import { createRouter, createWebHistory } from 'vue-router'


const routes = [
  { path: '/', name: 'Homepage', component: Homepage },
]

const router = createRouter({
  history: createWebHistory(),
  routes,
})

export default router