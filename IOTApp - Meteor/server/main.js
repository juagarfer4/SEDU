import { Meteor } from 'meteor/meteor';
import { Muestras } from '../imports/api/muestras';

Meteor.startup(() => {
  // code to run on server at startup
});

/*
Meteor.publish('muestras', ()=>{
  return Muestras.find();
})
*/

Meteor.methods({
  'muestras.get.fitngrow'({ results }) {
    const result = HTTP.call('GET', 'https://api.thingspeak.com/channels/283234/feeds.json', {
      params: { results: results }
    })
    // console.log(result);
    return result.data.feeds;
  },
  'muestras.get.wordstojoin'({ results }) {
    const result = HTTP.call('GET', 'https://api.thingspeak.com/channels/283217/feeds.json?api_key=APMYAASGZ249UAR9', {
      params: { results: results }
    })
    // console.log(result);
    return result.data.feeds;
  }
});