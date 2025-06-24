import { ref } from "vue";

const state = {
    busTotal : ref(1),
    busActive : ref(0),
    distanceToNextBus : ref(""),
    lastUpdate : ref("")
}

export default state;